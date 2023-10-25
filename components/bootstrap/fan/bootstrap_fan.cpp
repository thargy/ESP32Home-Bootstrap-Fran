#include "bootstrap_fan.h"
#include "esphome/core/log.h"

namespace esphome {
namespace bootstrap {

static const char *const TAG = "bootstrap.fan";

void BootstrapFan::setup() {
  auto restore = this->restore_state_();
  if (restore.has_value()) {
    restore->apply(*this);
    this->write_state_();
  }
}
void BootstrapFan::dump_config() { LOG_FAN("", "Bootstrap Fan", this); }
fan::FanTraits BootstrapFan::get_traits() {
  return fan::FanTraits(this->oscillating_ != nullptr, true, this->direction_ != nullptr, this->speed_count_);
}
void BootstrapFan::control(const fan::FanCall &call) {
  if (call.get_state().has_value()) {
    bool new_state = *call.get_state();
    if (new_state ) {
        if (!this->state && this->duration_ > 0) {
          // Changing from off to on, so need to bootstrap fan.
          this->booting=true;
          this->set_timeout(TAG, this->duration_, [&]{this->on_booted_();});
          ESP_LOGD(TAG, "Booting fan to %u for %u ms.", this->initial_value_, this->duration_);
      }
    } else {
      // If fan switched off, even whilst booting we stop the boot.
      this->cancel_timeout(TAG);
      this->booting=false;
    }
    this->state = new_state;
  }
  if (call.get_speed().has_value()) 
      this->speed = *call.get_speed();
  if (call.get_oscillating().has_value())
    this->oscillating = *call.get_oscillating();
  if (call.get_direction().has_value())
    this->direction = *call.get_direction();

  this->write_state_();
  this->publish_state();
}
void BootstrapFan::write_state_() {
  float speed = 0.0f;
  if (this->state) {
    int s = this->speed;
    if (this->booting) {
      int i = this->initial_value_;
      if (!this->use_higher_ || (i > s))
        s = i;
    } else {
       int min = this->minimum_value_;
       int max = this->maximum_value_;
       if (s < min) s = min;
       else if (s > max) s =max;
    }
    speed = static_cast<float>(s) / static_cast<float>(this->speed_count_);
  }
  this->output_->set_level(speed);

  if (this->oscillating_ != nullptr)
    this->oscillating_->set_state(this->oscillating);
  if (this->direction_ != nullptr)
    this->direction_->set_state(this->direction == fan::FanDirection::REVERSE);
}
void BootstrapFan::on_booted_() {
  ESP_LOGD(TAG, "Fan boot complete setting speed to %u.", this->speed);
  this->booting = false;
  this->write_state_();
}

}  // namespace bootstrap
}  // namespace esphome