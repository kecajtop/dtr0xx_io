#include "dtr0xx_io.h"
#include "esphome/core/log.h"

#ifdef USE_ESP32
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#endif

namespace esphome {
namespace dtr0xx_io {

static const char *const TAG = "dtr0xx_io";

#ifdef USE_ESP32
// Guards the bit-bang shift sequence in read_gpio_() so a FreeRTOS task
// switch or interrupt (e.g. the Wi-Fi stack) can't land mid-pulse-train and
// corrupt the timing of PL/CLK/RCK. That kind of jitter is a second,
// independent way to get glitchy/noisy latch pulses on the relay driver
// besides plain pulse width, and was reported to also fix the buzzing -
// see https://github.com/alexbirlica/dtr0xx_io/commit/bc91d039.
static portMUX_TYPE dtr0xx_io_gpio_mux = portMUX_INITIALIZER_UNLOCKED;
#define DTR0XX_IO_ENTER_CRITICAL() taskENTER_CRITICAL(&dtr0xx_io_gpio_mux)
#define DTR0XX_IO_EXIT_CRITICAL() taskEXIT_CRITICAL(&dtr0xx_io_gpio_mux)
#else
#define DTR0XX_IO_ENTER_CRITICAL()
#define DTR0XX_IO_EXIT_CRITICAL()
#endif

void dtr0xx_ioComponent::setup() {
  ESP_LOGCONFIG(TAG, "Setting up dtr0xx_io...");

  // Output-side pins are always needed.
  this->dingtian_clk_pin_->setup();
  this->dingtian_sdi_pin_->setup();
  this->dingtian_clk_pin_->digital_write(false);

  // Input-side pins (PL / Q7) are only needed if this instance reads inputs.
  if (this->use_input_) {
    this->dingtian_q7_pin_->setup();
    this->dingtian_pl_pin_->setup();
    // Set PL true before the first read so we don't briefly latch garbage
    // into the output register and flicker the relays on boot.
    this->dingtian_pl_pin_->digital_write(true);
  }

  if (this->dingtian_rck_pin_ != nullptr) {
    this->dingtian_rck_pin_->setup();
    this->dingtian_rck_pin_->digital_write(true);
  }

  // Push out the initial (all-off) output state / read the initial inputs.
  this->read_gpio_();
}

void dtr0xx_ioComponent::update() {
  if (!this->use_input_) {
    // No inputs configured on this instance: outputs are already latched
    // on demand from digital_write_(), so there is nothing to poll here.
    // Skipping this means we never re-clock the shift register on a timer,
    // which is what was causing the relays to buzz (see
    // kecajtop/dtr0xx_io#22).
    return;
  }
  this->read_gpio_();
}

void dtr0xx_ioComponent::dump_config() {
  ESP_LOGCONFIG(TAG, "dtr0xx_io:");
  ESP_LOGCONFIG(TAG, "  Inputs enabled: %s", this->use_input_ ? "YES" : "NO");
  if (this->use_input_) {
    ESP_LOGCONFIG(TAG, "  Input poll interval: %ums", this->get_update_interval());
  }
}

bool dtr0xx_ioComponent::digital_read_(uint16_t pin) {
  if (pin >= this->sr_count_ * 8) {
    ESP_LOGE(TAG, "Pin %u is out of range! Maximum pin number with %u chips in series is %u", pin, this->sr_count_,
             (this->sr_count_ * 8) - 1);
    return false;
  }
  return this->input_bits_[pin];
}

void dtr0xx_ioComponent::digital_write_(uint16_t pin, bool value) {
  if (pin >= this->sr_count_ * 8) {
    ESP_LOGE(TAG, "Pin %u is out of range! Maximum pin number with %u chips in series is %u", pin, this->sr_count_,
             (this->sr_count_ * 8) - 1);
    return;
  }
  this->output_bits_[pin] = value;
  this->read_gpio_();
}

void dtr0xx_ioComponent::read_gpio_() {
  // Nothing here should be interrupted by another task/ISR mid-sequence -
  // see the comment on dtr0xx_io_gpio_mux above.
  DTR0XX_IO_ENTER_CRITICAL();

  if (this->dingtian_rck_pin_ != nullptr)
    this->dingtian_rck_pin_->digital_write(false);

  if (this->use_input_) {
    // Pulse PL (SH/LD) to latch the current input state into the 165
    // register before shifting it out below. Left alone entirely when
    // use_input_ is false, since toggling this pin on every cycle was the
    // main source of buzz for output-only setups.
    this->dingtian_pl_pin_->digital_write(true);
    delayMicroseconds(10);
  }

  for (uint8_t i = 0; i < this->sr_count_; i++) {
    for (uint8_t j = 0; j < 8; j++) {
      if (this->use_input_) {
        this->input_bits_[(i * 8) + (7 - j)] = this->dingtian_q7_pin_->digital_read();
      }
      this->dingtian_sdi_pin_->digital_write(this->output_bits_[(i * 8) + (7 - j)]);
      this->dingtian_clk_pin_->digital_write(true);
      delayMicroseconds(10);
      this->dingtian_clk_pin_->digital_write(false);
      delayMicroseconds(10);
    }
  }

  if (this->use_input_) {
    this->dingtian_pl_pin_->digital_write(false);
  }

  if (this->dingtian_rck_pin_ != nullptr) {
    // Widen the storage-register clock pulse. The original code toggled
    // RCK true->false back-to-back with no delay, giving the 595's storage
    // register an extremely narrow (borderline) latch pulse - an
    // independent contributor to the buzz on top of any RTOS timing jitter.
    this->dingtian_rck_pin_->digital_write(true);
    delayMicroseconds(10);
  }

  DTR0XX_IO_EXIT_CRITICAL();
}

float dtr0xx_ioComponent::get_setup_priority() const { return setup_priority::IO; }

bool dtr0xx_ioGPIOPin::digital_read() { return this->parent_->digital_read_(this->pin_) != this->inverted_; }

void dtr0xx_ioGPIOPin::digital_write(bool value) {
  this->parent_->digital_write_(this->pin_, value != this->inverted_);
}

size_t dtr0xx_ioGPIOPin::dump_summary(char *buffer, size_t len) const {
  return buf_append_printf(buffer, len, 0, "%u via dtr0xx_io", pin_);
}

}  // namespace dtr0xx_io
}  // namespace esphome
