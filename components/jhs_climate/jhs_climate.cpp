#include "jhs_climate.h"
#include "esphome/components/rmt/rmt_component.h"
#include "esphome/core/log.h"

namespace esphome {
namespace jhs_climate {

static const char *TAG = "jhs_climate";

void JHSClimate::setup() {
  ESP_LOGI(TAG, "Setting up JHSClimate");
  setup_rmt();
  // Original setup logic falls vorhanden
}

void JHSClimate::setup_rmt() {
  if (rmt_component_ == nullptr) {
    rmt_component_ = new rmt::RMTComponent();
    rmt_component_->set_clk_div(80);  // 1 MHz Takt
    rmt_component_->set_mem_block_num(1);
    rmt_component_->setup();
  }

  // Pins anlegen (GPIO aus YAML oder config via get_pin())
  ac_tx_pin_ = new rmt::RMTPin(this->get_pin("ac_tx_pin"), rmt::RMT_TX_CHANNEL_0);
  panel_tx_pin_ = new rmt::RMTPin(this->get_pin("panel_tx_pin"), rmt::RMT_TX_CHANNEL_1);
  panel_rx_pin_ = new rmt::RMTPin(this->get_pin("panel_rx_pin"), rmt::RMT_RX_CHANNEL_0);

  // Channels anlegen
  rmt_ac_tx_channel_ = new rmt::RMTChannel(rmt::RMT_TX_CHANNEL_0);
  rmt_panel_tx_channel_ = new rmt::RMTChannel(rmt::RMT_TX_CHANNEL_1);
  rmt_rx_channel_ = new rmt::RMTChannel(rmt::RMT_RX_CHANNEL_0);

  // Channels konfigurieren
  rmt_ac_tx_channel_->set_pin(*ac_tx_pin_);
  rmt_panel_tx_channel_->set_pin(*panel_tx_pin_);
  rmt_rx_channel_->set_pin(*panel_rx_pin_);

  rmt_ac_tx_channel_->set_receive(false);
  rmt_panel_tx_channel_->set_receive(false);
  rmt_rx_channel_->set_receive(true);

  // Idle level konfigurieren
  rmt_ac_tx_channel_->set_idle_level(false);
  rmt_panel_tx_channel_->set_idle_level(false);
  rmt_rx_channel_->set_idle_level(false);

  // Channels mit Komponente verbinden
  rmt_component_->add_channel(rmt_ac_tx_channel_);
  rmt_component_->add_channel(rmt_panel_tx_channel_);
  rmt_component_->add_channel(rmt_rx_channel_);

  // Channels starten (enable)
  rmt_ac_tx_channel_->enable();
  rmt_panel_tx_channel_->enable();
  rmt_rx_channel_->enable();
}


void JHSClimate::send_ac_command(const std::vector<uint8_t> &data) {
  ESP_LOGD(TAG, "send_ac_command: %s", format_hex_pretty(data).c_str());
  rmt_ac_tx_channel_->write_bytes(data.data(), data.size());
  rmt_ac_tx_channel_->wait_done(100);
}

void JHSClimate::send_panel_command(const std::vector<uint8_t> &data) {
  ESP_LOGD(TAG, "send_panel_command: %s", format_hex_pretty(data).c_str());
  rmt_panel_tx_channel_->write_bytes(data.data(), data.size());
  rmt_panel_tx_channel_->wait_done(100);
}

void JHSClimate::recv_from_panel() {
  std::vector<uint8_t> buffer(64);
  size_t length = 0;

  if (!rmt_rx_channel_->read_bytes(buffer.data(), buffer.size(), &length, 100)) {
    // keine Daten empfangen
    return;
  }
  if (length == 0) {
    return;
  }

  std::vector<uint8_t> frame(buffer.begin(), buffer.begin() + length);

  if (frame.size() != 13) {
    ESP_LOGW(TAG, "recv_from_panel: Ungültige Framegröße %d", (int)frame.size());
    return;
  }

  ESP_LOGI(TAG, "recv_from_panel: Frame empfangen: %s", format_hex_pretty(frame).c_str());
  parse_panel_frame(frame);
}

void JHSClimate::parse_panel_frame(const std::vector<uint8_t> &frame) {
  if (frame.size() != 13) {
    ESP_LOGW(TAG, "parse_panel_frame: Ungültige Framegröße");
    return;
  }

  uint8_t checksum = 0;
  for (int i = 0; i < 12; i++) {
    checksum += frame[i];
  }
  checksum &= 0xFF;

  if (checksum != frame[12]) {
    ESP_LOGW(TAG, "parse_panel_frame: Checksum mismatch (calc=%02X recv=%02X)", checksum, frame[12]);
    return;
  }

  uint8_t mode = (frame[5] & 0x70) >> 4;
  uint8_t fan = frame[5] & 0x0F;
  uint8_t temp = frame[6];

  ESP_LOGI(TAG, "parse_panel_frame: Mode=%d Fan=%d Temp=%d Checksum=%02X", mode, fan, temp, checksum);

  this->mode_ = mode;
  this->fan_ = fan;
  this->target_temperature_ = temp;

  // Logik aus Original: Status aktualisieren, evtl. state in ESPHome Climate aktualisieren
  this->publish_state();
}

void JHSClimate::recv_from_ac() {
  std::vector<uint8_t> buffer(64);
  size_t length = 0;

  if (!rmt_rx_channel_->read_bytes(buffer.data(), buffer.size(), &length, 100)) {
    return;
  }
  if (length == 0) {
    return;
  }

  std::vector<uint8_t> frame(buffer.begin(), buffer.begin() + length);

  if (frame.size() != 13) {
    ESP_LOGW(TAG, "recv_from_ac: Ungültige Framegröße %d", (int)frame.size());
    return;
  }

  ESP_LOGI(TAG, "recv_from_ac: Frame empfangen: %s", format_hex_pretty(frame).c_str());
  parse_ac_frame(frame);
}

void JHSClimate::parse_ac_frame(const std::vector<uint8_t> &frame) {
  if (frame.size() != 13) {
    ESP_LOGW(TAG, "parse_ac_frame: Ungültige Framegröße");
    return;
  }

  uint8_t checksum = 0;
  for (int i = 0; i < 12; i++) {
    checksum += frame[i];
  }
  checksum &= 0xFF;

  if (checksum != frame[12]) {
    ESP_LOGW(TAG, "parse_ac_frame: Checksum mismatch (calc=%02X recv=%02X)", checksum, frame[12]);
    return;
  }

  // Original-Logik zur Verarbeitung AC-Frame hier übernehmen
  ESP_LOGI(TAG, "parse_ac_frame: AC Frame valid");
}

void JHSClimate::loop() {
  recv_from_panel();
  recv_from_ac();

  // Zusätzliche Original-Logik aus dem Loop, z.B. Anpassungen, Timings, Display-Updates

  if (steps_left_to_adjust_mode_ > 0) {
    steps_left_to_adjust_mode_--;
    // Mode anpassen falls nötig
  }
  if (steps_left_to_adjust_temp_ > 0) {
    steps_left_to_adjust_temp_--;
    // Temperatur anpassen falls nötig
  }
  if (steps_left_to_adjust_fan_ > 0) {
    steps_left_to_adjust_fan_--;
    // Lüfter anpassen falls nötig
  }

  // Beispiel für Display-Update-Aufruf:
  update_screen_if_needed();
}

void JHSClimate::update_screen_if_needed() {
  // Original Logik übernehmen, z.B. Statusanzeige auf Display aktualisieren
}

void JHSClimate::dump_config() {
  ESP_LOGCONFIG(TAG, "JHS Climate Komponente:");
  ESP_LOGCONFIG(TAG, "AC TX Pin: %d", this->ac_tx_pin_->get_pin());
  ESP_LOGCONFIG(TAG, "Panel TX Pin: %d", this->panel_tx_pin_->get_pin());
  ESP_LOGCONFIG(TAG, "Panel RX Pin: %d", this->panel_rx_pin_->get_pin());
}

climate::ClimateTraits JHSClimate::traits() {
  auto traits = climate::ClimateTraits();
  traits.set_supports_cool_mode(true);
  traits.set_supports_heat_mode(true);
  traits.set_supports_fan_only_mode(true);
  traits.set_supports_auto_mode(true);
  traits.set_supports_off_mode(true);
  traits.set_supports_fan_modes({"AUTO", "LOW", "MEDIUM", "HIGH"});
  traits.set_supports_current_temperature(true);
  return traits;
}

void JHSClimate::control(const climate::ClimateCall &call) {
  if (call.get_mode().has_value()) {
    this->mode_ = static_cast<uint8_t>(call.get_mode().value());
    this->steps_left_to_adjust_mode_ = 3;
  }
  if (call.get_target_temperature().has_value()) {
    this->target_temperature_ = static_cast<int8_t>(call.get_target_temperature().value());
    this->steps_left_to_adjust_temp_ = 3;
  }
  if (call.get_fan_mode().has_value()) {
    this->fan_ = static_cast<uint8_t>(call.get_fan_mode().value());
    this->steps_left_to_adjust_fan_ = 3;
  }
}

bool JHSClimate::is_adjusting() {
  return steps_left_to_adjust_mode_ > 0 || steps_left_to_adjust_temp_ > 0 || steps_left_to_adjust_fan_ > 0 || adjust_preset_;
}

}  // namespace jhs_climate
}  // namespace esphome
