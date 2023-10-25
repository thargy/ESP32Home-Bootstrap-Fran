#pragma once

#include "esphome/core/component.h"
#include "esphome/components/output/binary_output.h"
#include "esphome/components/output/float_output.h"
#include "esphome/components/fan/fan.h"

namespace esphome {
namespace bootstrap {

class BootstrapFan : public Component, public fan::Fan {
 public:
  BootstrapFan(output::FloatOutput *output, int speed_count_, int duration_, int initial_value_, int minimum_value_, int maxiumum_value_, bool use_higher_) : output_(output), speed_count_(speed_count_), duration_(duration_), initial_value_(initial_value_),
    minimum_value_(minimum_value_), maximum_value_(maxiumum_value_), use_higher_(use_higher_) {}
  void setup() override;
  void dump_config() override;
  void set_oscillating(output::BinaryOutput *oscillating) { this->oscillating_ = oscillating; }
  void set_direction(output::BinaryOutput *direction) { this->direction_ = direction; }
  fan::FanTraits get_traits() override;

 protected:
  void control(const fan::FanCall &call) override;
  void write_state_();
  void on_booted_();

  output::FloatOutput *output_;
  output::BinaryOutput *oscillating_{nullptr};
  output::BinaryOutput *direction_{nullptr};
  int speed_count_{};
  int duration_{};
  bool booting{false};
  int initial_value_{};
  int minimum_value_{};
  int maximum_value_{};
  bool use_higher_{};
};

}  // namespace bootstrap
}  // namespace esphome