#include "dtr0xx_io.h"
#include "esphome/core/log.h"

namespace esphome {
namespace dtr0xx_io {

static const char *const TAG = "dtr0xx_io";

void dtr0xx_ioComponent::setup() {
  ESP_LOGCONFIG(TAG, "Setting up dtr0xx_io...");

  // initialize pins
  this->dingtian_clk_pin_->setup();
  this->dingtian_q7_pin_->setup();
  this->dingtian_sdi_pin_->setup();
  this->dingtian_pl_pin_->setup();

  this->dingtian_clk_pin_->digital_write(false);
  this->dingtian_pl_pin_->digital_write(false);

  if (this->dingtian_rck_pin_ != nullptr) {
    this->dingtian_rck_pin_->setup();
    this->dingtian_rck_pin_->digital_write(true);
  }

  // read state from shift register
  this->read_gpio_();
}

void dtr0xx_ioComponent::loop() { this->read_gpio_(); }

void dtr0xx_ioComponent::dump_config() { ESP_LOGCONFIG(TAG, "dtr0xx_io:"); }

bool dtr0xx_ioComponent::digital_read_(uint16_t pin) {
  if (pin >= this->sr_count_ * 8) {
    ESP_LOGE(TAG, "Pin %u is out of range! Maximum pin number with %u chips in series is %u", pin, this->sr_count_,
             (this->sr_count_ * 8) - 1);
    return false;
  }
  return this->input_bits_[pin];
}

void dtr0xx_ioComponent::digital_write_(uint16_t pin, bool value)
{
    if (pin >= this->sr_count_ * 8) {
    ESP_LOGE(TAG, "Pin %u is out of range! Maximum pin number with %u chips in series is %u", pin, this->sr_count_,
             (this->sr_count_ * 8) - 1);
    return;
  }
  this->output_bits_[pin] = value;
  this->read_gpio_();
}

void dtr0xx_ioComponent::read_gpio_() {

  this->dingtian_pl_pin_->digital_write(true);
  delayMicroseconds(10);

  if (this->dingtian_rck_pin_ != nullptr)
    this->dingtian_rck_pin_->digital_write(false);

  for (uint8_t i = 0; i < this->sr_count_; i++) {
    for (uint8_t j = 0; j < 8; j++) {
      this->input_bits_[(i * 8) + (7 - j)] = this->dingtian_q7_pin_->digital_read();
      this->dingtian_sdi_pin_->digital_write(this->output_bits_[(i * 8) + (7 - j)]);
      this->dingtian_clk_pin_->digital_write(true);
      delayMicroseconds(10);
      this->dingtian_clk_pin_->digital_write(false);
      delayMicroseconds(10);
    }
  }
  this->dingtian_pl_pin_->digital_write(false);
  //if (this->dingtian_rck_pin_ != nullptr)
    this->dingtian_rck_pin_->digital_write(true);
    this->dingtian_rck_pin_->digital_write(false);
}

float dtr0xx_ioComponent::get_setup_priority() const { return setup_priority::IO; }

bool dtr0xx_ioGPIOPin::digital_read() { 
  return this->parent_->digital_read_(this->pin_) != this->inverted_; }

void dtr0xx_ioGPIOPin::digital_write(bool value) {
  this->parent_->digital_write_(this->pin_, value != this->inverted_);}

std::string dtr0xx_ioGPIOPin::dump_summary() const { return str_snprintf("%u via dtr0xx_io", 18, pin_); }

}  // namespace dtr0xx_io
}  // namespace esphome
