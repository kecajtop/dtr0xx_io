#pragma once

#include "esphome/core/component.h"
#include "esphome/core/hal.h"
#include "esphome/core/helpers.h"
#include "esphome/components/spi/spi.h"

namespace esphome {
namespace dtr008v2io {

class dtr008v2ioComponent : public Component,
                            public spi::SPIDevice<spi::BIT_ORDER_MSB_FIRST,
                                                  spi::CLOCK_POLARITY_LOW,
                                                  spi::CLOCK_PHASE_LEADING,
                                                  spi::DATA_RATE_4MHZ> {
 public:
  static constexpr uint8_t segment_pins = 8;
  dtr008v2ioComponent() = default;

  void setup() override;
  void loop() override;
  float get_setup_priority() const override;
  void dump_config() override;

  void set_oe_pin(GPIOPin *pin) { this->oe_pin_ = pin; }
  void set_use_inputs() { this->use_inputs_ = true; }

 protected:
  friend class dtr008v2ioGPIOPin;
  bool digital_read_(uint16_t pin);
  void digital_write_(uint16_t pin, bool value);
  void transfer_gpio_();

  GPIOPin *oe_pin_;
  uint8_t input_byte_;
  uint8_t output_byte_;
  bool use_inputs_;
};

/// Helper class to expose a SN74HC165/595 pin as an internal GPIO pin.
class dtr008v2ioGPIOPin : public GPIOPin, public Parented<dtr008v2ioComponent> {
 public:
  void setup() override {}
  void pin_mode(gpio::Flags flags) override {}
  bool digital_read() override;
  void digital_write(bool value) override;
  std::string dump_summary() const override;

  void set_pin(uint16_t pin) { pin_ = pin; }
  void set_inverted(bool inverted) { inverted_ = inverted; }

  void set_flags(gpio::Flags flags) { this->flags_ = flags; }
  gpio::Flags get_flags() const override { return this->flags_; }

 protected:
  uint16_t pin_;
  bool inverted_;
  gpio::Flags flags_;
};

}  // namespace dtr008v2io
}  // namespace esphome
