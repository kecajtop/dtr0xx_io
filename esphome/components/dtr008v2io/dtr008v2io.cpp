#include "dtr008v2io.h"
#include "esphome/core/log.h"

namespace esphome {
namespace dtr008v2io {

static const char *const TAG = "dtr008v2io";

void dtr008v2ioComponent::setup() {
  ESP_LOGCONFIG(TAG, "Setting up dtr008v2io...");

  this->oe_pin_->setup();
  this->oe_pin_->digital_write(true);

  this->spi_setup();

  this->transfer_gpio_();
}


void dtr008v2ioComponent::loop() {
  if (this->use_inputs_) {
    this->transfer_gpio_();
  }
}

void dtr008v2ioComponent::dump_config() { ESP_LOGCONFIG(TAG, "dtr008v2io:"); }

void dtr008v2ioComponent::digital_write_(uint16_t pin, bool value) {
  if (pin >= segment_pins) {
    ESP_LOGE(TAG, "Invalid output pin %u (0..%d)!", pin, segment_pins-1);
    return;
  }
  uint8_t current = output_byte_;
  if (value) {
    this->output_byte_ |= (1 << (7-pin));
  } else {
    this->output_byte_ &= ~(1 << (7-pin));
  }
  if (!this->use_inputs_ && (current != output_byte_)) {
    this->transfer_gpio_();
  }
}

bool dtr008v2ioComponent::digital_read_(uint16_t pin) {
  if (pin >= segment_pins) {
    ESP_LOGE(TAG, "Invalid input pin %u (0..%d)!", pin, segment_pins-1);
    return false;
  }
  return (bool)(this->input_byte_ & (1 << (7-pin)));
}

void dtr008v2ioComponent::transfer_gpio_() {
  this->enable();
  input_byte_ = this->transfer_byte(output_byte_);
  this->disable();

  this->oe_pin_->digital_write(false);
}

float dtr008v2ioComponent::get_setup_priority() const { return setup_priority::IO; }

bool dtr008v2ioGPIOPin::digital_read() {
  return this->parent_->digital_read_(this->pin_) != this->inverted_;
}

void dtr008v2ioGPIOPin::digital_write(bool value) {
  this->parent_->digital_write_(this->pin_, value != this->inverted_);
}

std::string dtr008v2ioGPIOPin::dump_summary() const { return str_snprintf("%u via dtr008v2io", 16, pin_); }

}  // namespace dtr008v2io
}  // namespace esphome
