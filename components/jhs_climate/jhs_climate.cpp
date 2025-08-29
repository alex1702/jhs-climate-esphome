#include "jhs_climate.h"


static const char *TAG = "JHSClimateComp";


namespace JHS {


    void JHSClimate::setup() {
        ESP_LOGI(TAG, "JHSClimate component initialisiert");
    }


    void JHSClimate::dump_config() {
        ESP_LOGCONFIG(TAG, "JHSClimate:");
    }


    void JHSClimate::loop() {
        uint8_t b;
        while (this->available()) {
            if (this->read_byte(&b)) {
                buffer_.push_back(b);
                // Packet length = sizeof(JHSAcPacket) + checksum
                const size_t packet_len = sizeof(JHSAcPacket) + 1;
                if (buffer_.size() >= packet_len) {
                    // Versuche Parsing mit allen ersten packet_len Bytes
                    std::vector<uint8_t> candidate(buffer_.begin(), buffer_.begin() + packet_len);
                    auto pkt = JHSAcPacket::parse(candidate);
                    if (pkt.has_value()) {
                        ESP_LOGI(TAG, "Paket erkannt: %s", pkt->to_string().c_str());
                        // Entferne die ersten packet_len Bytes
                        buffer_.erase(buffer_.begin(), buffer_.begin() + packet_len);
                    } else {
                        // kein gültiges Paket: Schiebe Fenster um 1 Byte
                        buffer_.erase(buffer_.begin());
                    }
                }
            }
        }
    }


} // namespace JHS