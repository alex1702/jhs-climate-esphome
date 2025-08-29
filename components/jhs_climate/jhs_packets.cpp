#include "jhs_packets.h"


int JHSAcPacket::get_temp()
{
char d1 = seven_segment_to_char(first_digit);
char d2 = seven_segment_to_char(second_digit);
if (d1 > '9' || d2 > '9' || d1 < '0' || d2 < '0')
return -1;
return (d1 - '0') * 10 + (d2 - '0');
}


void JHSAcPacket::set_temp(int temp)
{
if (temp < 0 || temp > 99)
return;
first_digit = char_to_seven_segment(temp / 10 + '0');
second_digit = char_to_seven_segment(temp % 10 + '0');
}


void JHSAcPacket::set_display(std::string temp)
{
if (temp.size() != 2)
return;
first_digit = char_to_seven_segment(temp[0]);
second_digit = char_to_seven_segment(temp[1]);
}


esphome::optional<JHSAcPacket> JHSAcPacket::parse(const std::vector<uint8_t> &data)
{


if (data.size() != sizeof(JHSAcPacket) + 1)
{
ESP_LOGE(TAG, "Invalid packet size: %d, should be %d", data.size(), sizeof(JHSAcPacket) + 1);
return esphome::optional<JHSAcPacket>();
}
JHSAcPacket packet;
std::memcpy(&packet, data.data(), sizeof(JHSAcPacket));
uint8_t checksum = data.back();
uint8_t checksum_calculated = 90;
for (size_t i = 0; i < data.size() - 1; i++)
{
checksum_calculated += data[i];
}
if (checksum != checksum_calculated)
{
ESP_LOGV(TAG, "JHS AC packet checksum mismatch: %d != %d in packet: %s", checksum, checksum_calculated, bytes_to_hex(data).c_str());
return esphome::optional<JHSAcPacket>();
}
return esphome::make_optional(packet);
}


std::vector<uint8_t> JHSAcPacket::to_wire_format()
{
std::vector<uint8_t> data;
data.resize(sizeof(JHSAcPacket) + 1);
std::memcpy(data.data(), &this->addr, sizeof(JHSAcPacket));
uint8_t checksum = 90;
for (size_t i = 0; i < data.size() - 1; i++)
{
checksum += data[i];
}
data.back() = checksum;
return data;
}