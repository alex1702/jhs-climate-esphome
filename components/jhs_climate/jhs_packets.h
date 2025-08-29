#pragma once

#include "esphome.h"
#include <vector>
#include <string>

namespace JHS {

// Paketstruktur vom AC
struct JHSAcPacket {
  uint8_t addr = 0x90;  // immer 0x90
  uint8_t first_digit = 0;
  uint8_t second_digit = 0;
  uint8_t zero0 = 0;
  uint8_t zero1 = 0;
  uint8_t cool : 1 = 0;
  uint8_t dehum : 1 = 0;
  uint8_t fan : 1 = 0;
  uint8_t heat : 1 = 0;
  uint8_t sleep : 1 = 0;
  uint8_t water_full : 1 = 0;
  uint8_t swing : 1 = 0;
  uint8_t timer : 1 = 0;
  uint8_t fan_low : 1 = 0;
  uint8_t fan_unused : 1 = 0;
  uint8_t fan_high : 1 = 0;
  uint8_t wifi : 1 = 0;
  uint8_t unused_above_timer : 1 = 0;
  uint8_t power : 1 = 1;
  uint8_t unused4 : 1 = 0;
  uint8_t unused5 : 1 = 0;
  uint8_t beep_length : 4 = 0;
  uint8_t beep_amount : 4 = 0;

  std::string to_string() {
    std::stringstream ss;
    ss << "DISP:" << (int)first_digit << (int)second_digit;
    if (power) ss << " POWER";
    if (cool) ss << " COOL";
    if (heat) ss << " HEAT";
    if (fan) ss << " FAN";
    if (dehum) ss << " DEHUM";
    return ss.str();
  }

  static JHSAcPacket parse(const std::vector<uint8_t> &data) {
    JHSAcPacket p;
    if (data.size() < sizeof(JHSAcPacket) + 1) return p;
    memcpy(&p, data.data(), sizeof(JHSAcPacket));
    return p;
  }
} __attribute__((packed));

}  // namespace JHS
