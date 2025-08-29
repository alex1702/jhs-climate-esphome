#include "jhs_packets.h"
#include "esphome/core/log.h"
#include <cstring>
#include <sstream>

namespace JHS {

static const char *TAG = "JHSClimate";

// Hilfsfunktion: Paket in lesbare String-Darstellung
std::string JHSAcPacket::to_string() {
  std::stringstream ss;
  ss << "DISP:" << (int)first_digit << (int)second_digit;
  if (power) ss << " POWER";
  if (cool) ss << " COOL";
  if (heat) ss << " HEAT";
  if (fan) ss << " FAN";
  if (dehum) ss << " DEHUM";
  if (sleep) ss << " SLEEP";
  if (water_full) ss << " WATERFULL";
  if (timer) ss << " TIMER";
  if (beep_amount > 0 && beep_length > 0) {
    ss << " BEEP " << (int)beep_amount << "x " << (int)beep_length;
  }
  return ss.str();
}

// Einfacher Parser: kopiert die Daten in die Struktur
JHSAcPacket JHSAcPacket::parse(const std::vector<uint8_t> &data) {
  JHSAcPacket pkt;
  if (data.size() < sizeof(JHSAcPacket) + 1) {
    ESP_LOGW(TAG, "Packet too short (%d bytes)", (int)data.size());
    return pkt;
  }
  std::memcpy(&pkt, data.data(), sizeof(JHSAcPacket));
  return pkt;
}

}  // namespace JHS
