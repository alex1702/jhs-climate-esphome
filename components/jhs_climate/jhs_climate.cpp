#include "jhs_climate.h"

static const char *TAG = "JHSClimateComp";

namespace JHS {

void JHSClimate::setup() {
  ESP_LOGI(TAG, "JHSClimate component initialized");
}

void JHSClimate::update() {
  uint8_t b;
  while (this->available()) {
    if (this->read_byte(&b)) {
      buffer_.push_back(b);
      const size_t packet_len = sizeof(JHSAcPacket) + 1;
      if (buffer_.size() >= packet_len) {
        std::vector<uint8_t> candidate(buffer_.begin(), buffer_.begin() + packet_len);
        auto pkt = JHSAcPacket::parse(candidate);
        if (pkt.has_value()) {
          ESP_LOGI(TAG, "Paket erkannt: %s", pkt->to_string().c_str());
          buffer_.erase(buffer_.begin(), buffer_.begin() + packet_len);
        } else {
          buffer_.erase(buffer_.begin());
        }
      }
    }
  }
}

} // namespace JHS
