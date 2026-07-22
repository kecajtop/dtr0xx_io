#pragma once

#include "esphome/core/component.h"
#include "esphome/core/hal.h"
#include "esphome/core/helpers.h"

#include <vector>

namespace esphome {
namespace dtr0xx_io {

class dtr0xx_ioComponent : public Component {
 public:
  dtr0xx_ioComponent() = default;

  void setup() override;
  void loop() override;
  float get_setup_priority() const override;
  void dump_config() override;

  void set_dingtian_q7_pin(GPIOPin *pin) { this->dingtian_q7_pin_ = pin; }
  void set_dingtian_sdi_pin(GPIOPin *pin) { this->dingtian_sdi_pin_ = pin; }

  void set_dingtian_clk_pin(GPIOPin *pin) { this->dingtian_clk_pin_ = pin; }
  void set_dingtian_pl_pin(GPIOPin *pin) { this->dingtian_pl_pin_ = pin; }
  void set_dingtian_rck_pin(GPIOPin *pin) { this->dingtian_rck_pin_ = pin; }

  /// Whether this instance actually reads inputs (SN74HC165 side) or only
  /// drives outputs (SN74HC595 side). When false, we never touch the PL
  /// (SH/LD) pin and never poll in loop() - this is what avoids the relay
  /// buzz reported for output-only setups.
  void set_use_input(bool use_input) { this->use_input_ = use_input; }
  /// How often (ms) to poll inputs in loop() when use_input is true.
  void set_update_interval(uint32_t update_interval) { this->update_interval_ = update_interval; }

  void set_sr_count(uint8_t count) {
    this->sr_count_ = count;
    this->input_bits_.resize(count * 8);
    this->output_bits_.resize(count * 8);
  }

 protected:
  friend class dtr0xx_ioGPIOPin;
  bool digital_read_(uint16_t pin);
  void digital_write_(uint16_t pin, bool value);
  void read_gpio_();
  void write_gpio_();

  GPIOPin *dingtian_q7_pin_{nullptr};
  GPIOPin *dingtian_sdi_pin_{nullptr};
  GPIOPin *dingtian_clk_pin_{nullptr};
  GPIOPin *dingtian_pl_pin_{nullptr};
  GPIOPin *dingtian_rck_pin_{nullptr};
  uint8_t sr_count_;
  bool use_input_{true};
  uint32_t update_interval_{100};
  uint32_t last_read_{0};
  std::vector<bool> input_bits_;
  std::vector<bool> output_bits_;
};

/// Helper class to expose a SN74HC165 pin as an internal input GPIO pin.
class dtr0xx_ioGPIOPin : public GPIOPin, public Parented<dtr0xx_ioComponent> {
 public:
  void setup() override {}
  void pin_mode(gpio::Flags flags) override {}
  bool digital_read() override;
  void digital_write(bool value) override;
  size_t dump_summary(char *buffer, size_t len) const override;

  void set_pin(uint16_t pin) { pin_ = pin; }
  void set_inverted(bool inverted) { inverted_ = inverted; }

  void set_flags(gpio::Flags flags) { this->flags_ = flags; }
  gpio::Flags get_flags() const override { return this->flags_; }

 protected:
  uint16_t pin_;
  bool inverted_;
  gpio::Flags flags_;
};

}  // namespace dtr0xx_io
}  // namespace esphome
