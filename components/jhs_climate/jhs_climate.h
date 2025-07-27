#pragma once

#include "esphome/core/component.h"
#include "esphome/components/climate/climate.h"
#include "driver/rmt_tx.h"
#include "driver/rmt_rx.h"
#include <vector>

namespace esphome {
namespace jhs_climate {

class JHSClimate : public climate::Climate, public Component {
 public:
  void setup() override;
  void loop() override;
  void dump_config() override;

  climate::ClimateTraits traits() override;
  void control(const climate::ClimateCall &call) override;

 protected:
  void setup_rmt();
  void send_ac_command(const std::vector<uint8_t> &data);
  void send_panel_command(const std::vector<uint8_t> &data);
  void recv_from_panel();
  void recv_from_ac();

  void parse_panel_frame(const std::vector<uint8_t> &frame);
  void parse_ac_frame(const std::vector<uint8_t> &frame);

  // Pins
  rmt::RMTPin *ac_tx_pin_{nullptr};
  rmt::RMTPin *panel_tx_pin_{nullptr};
  rmt::RMTPin *panel_rx_pin_{nullptr};

  rmt::RMTChannel *rmt_ac_tx_channel_{nullptr};
  rmt::RMTChannel *rmt_panel_tx_channel_{nullptr};
  rmt::RMTChannel *rmt_rx_channel_{nullptr};

  // State
  uint8_t mode_{0};
  uint8_t fan_{0};
  int8_t target_temperature_{0};

  int steps_left_to_adjust_mode_{0};
  int steps_left_to_adjust_fan_{0};
  int steps_left_to_adjust_temp_{0};
  bool adjust_preset_{false};

  rmt::RMTComponent *rmt_component_{nullptr};
};

}  // namespace jhs_climate
}  // namespace esphome
