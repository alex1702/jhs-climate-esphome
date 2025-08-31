#include "jhs_climate.h"

#include "jhs_recv_task.h"

static const char *TAG = "JHSClimate";

namespace JHS {

// JHSClimate::JHSClimate(int panel_tx_pin, int panel_rx_pin, int ac_tx_pin, int ac_rx_pin, uart_port_t uart_num)
//     : tx_pin_(panel_tx_pin), rx_pin_(panel_rx_pin), uart_num_(uart_num) {}

void JHSClimate::setup() {


    ESP_LOGI(TAG, "Setting up JHSClimate...");
    jhs_recv_task_config recv_config = {
        .ac_rx_pin = this->ac_rx_pin_,
        .panel_rx_pin = this->panel_rx_pin_};
    start_jhs_climate_recv_task(recv_config);
    ESP_LOGI(TAG, "JHSClimate setup complete");
}


void JHSClimate::loop() {
    // this->recv_from_ac();
    this->recv_from_panel();
}

void JHSClimate::recv_from_panel()
{
    uint8_t packet[JHS_PANEL_PACKET_SIZE];
    while (xQueueReceive(panel_rx_queue, &packet, 0))
    {
        ESP_LOGI("ISR", "ISR fired via queue!");
        // std::vector<uint8_t> packet_vector(packet, packet + JHS_PANEL_PACKET_SIZE);

        // if (memcmp(packet, &KEEPALIVE_PACKET, JHS_PANEL_PACKET_SIZE) == 0)
        // {
        //     ESP_LOGVV(TAG, "Received keepalive packet from panel");
        // }
        // else if (memcmp(packet, &BUTTON_ON, JHS_PANEL_PACKET_SIZE) == 0)
        // {
        //     ESP_LOGI(TAG, "Received BUTTON_ON from panel");
        // }
        // else if (memcmp(packet, &BUTTON_LOWER_TEMP, JHS_PANEL_PACKET_SIZE) == 0)
        // {
        //     ESP_LOGI(TAG, "Received BUTTON_LOWER_TEMP from panel");
        // }
        // else if (memcmp(packet, &BUTTON_HIGHER_TEMP, JHS_PANEL_PACKET_SIZE) == 0)
        // {
        //     ESP_LOGI(TAG, "Received BUTTON_HIGHER_TEMP from panel");
        // }
        // else if (memcmp(packet, &BUTTON_MODE, JHS_PANEL_PACKET_SIZE) == 0)
        // {
        //     ESP_LOGI(TAG, "Received BUTTON_MODE from panel");
        // }
        // else if (memcmp(packet, &BUTTON_FAN, JHS_PANEL_PACKET_SIZE) == 0)
        // {
        //     ESP_LOGI(TAG, "Received BUTTON_FAN from panel");
        // }
        // else if (memcmp(packet, &BUTTON_SLEEP, JHS_PANEL_PACKET_SIZE) == 0)
        // {
        //     ESP_LOGI(TAG, "Received BUTTON_SLEEP from panel");
        // }
        // else if (memcmp(packet, &BUTTON_TIMER, JHS_PANEL_PACKET_SIZE) == 0)
        // {
        //     ESP_LOGI(TAG, "Received BUTTON_TIMER from panel");
        // }
        // else if (memcmp(packet, &BUTTON_UNIT_CHANGE, JHS_PANEL_PACKET_SIZE) == 0)
        // {
        //     ESP_LOGI(TAG, "Received BUTTON_UNIT_CHANGE from panel, ignoring");
        //     JHSAcPacket hello_packet;
        //     hello_packet.beep_amount = 3;
        //     hello_packet.beep_length = 2;
        //     hello_packet.power = 0;
        //     hello_packet.cool = 1;
        //     hello_packet.set_display("dd");
        //     this->send_rmt_data(this->rmt_panel_tx, hello_packet.to_wire_format());
        //     continue;
        // }
        // else
        // {
        //     ESP_LOGI(TAG, "Received unknown packet from panel: %s", bytes_to_hex2(packet_vector).c_str());
        // }
        // this->send_rmt_data(this->rmt_ac_tx, packet_vector);
    }
}

// void JHSClimate::recv_from_ac()
// {
//     uint8_t packet[JHS_AC_PACKET_SIZE];

//     while (xQueueReceive(ac_rx_queue, &packet, 0))
//     {
//         // std::vector<uint8_t> packet_vector(packet, packet + JHS_AC_PACKET_SIZE);
//         // esphome::optional<JHSAcPacket> packet_optional = JHSAcPacket::parse(packet_vector);
//         // if (!packet_optional)
//         // {
//         //     ESP_LOGV(TAG, "Received invalid packet from AC");
//         //     continue;
//         // }
//         // JHSAcPacket packet = *packet_optional;
//         // ESP_LOGVV(TAG, "Received new packet from AC: %s", packet.to_string());


//         // // Modify the packet 
//         // packet.wifi = !wifi::global_wifi_component->is_connected();
//         // if (is_adjusting()){
//         //     packet.beep_amount = 0;
//         //     packet.beep_length = 0;
//         // }

//         // esphome::climate::ClimateMode mode_from_packet = esphome::climate::CLIMATE_MODE_OFF;
//         // if (packet.cool)
//         // {
//         //     mode_from_packet = esphome::climate::CLIMATE_MODE_COOL;
//         // }
//         // else if (packet.heat)
//         // {
//         //     mode_from_packet = esphome::climate::CLIMATE_MODE_HEAT;
//         // }
//         // else if (packet.fan)
//         // {
//         //     mode_from_packet = esphome::climate::CLIMATE_MODE_FAN_ONLY;
//         // }
//         // else if (packet.dehum)
//         // {
//         //     mode_from_packet = esphome::climate::CLIMATE_MODE_DRY;
//         // }
//         // esphome::climate::ClimateFanMode fan_from_packet = esphome::climate::CLIMATE_FAN_LOW;
//         // if (packet.fan_high)
//         // {
//         //     fan_from_packet = esphome::climate::CLIMATE_FAN_HIGH;
//         // }
//         // if (packet.fan_low)
//         // {
//         //     fan_from_packet = esphome::climate::CLIMATE_FAN_LOW;
//         // }
//         // esphome::climate::ClimatePreset preset_from_packet = esphome::climate::CLIMATE_PRESET_NONE;
//         // if (packet.sleep) preset_from_packet = esphome::climate::CLIMATE_PRESET_SLEEP;

//         // if (!this->is_adjusting())
//         // {
//         //     // if we are not adjusting anything we can copy the state from the packet to the climate

//         //     bool did_change = false;
//         //     if (packet.get_temp() > 0 && this->target_temperature != packet.get_temp() && packet.cool)
//         //     {
//         //         this->target_temperature = packet.get_temp();
//         //         did_change = true;
//         //     }
//         //     if (this->current_temperature != packet.get_temp())
//         //     {
//         //         this->current_temperature = packet.get_temp(); // Fake the current temperature
//         //         did_change = true;
//         //     }
//         //     if (this->mode != mode_from_packet)
//         //     {
//         //         this->mode = mode_from_packet;
//         //         did_change = true;
//         //     }
//         //     if (this->fan_mode != fan_from_packet)
//         //     {
//         //         this->fan_mode = fan_from_packet;
//         //         did_change = true;
//         //     }
//         //     if (this->preset != preset_from_packet)
//         //     {
//         //         this->preset = preset_from_packet;
//         //         did_change = true;
//         //     }
            
//         //     if (did_change)
//         //     {
//         //         this->publish_state();
//         //     }
//         //     if (this->water_full != packet.water_full)
//         //     {
//         //         if (packet.water_full){
//         //             last_water_full = esphome::millis();
//         //             this->water_full = true;
//         //         }else{
//         //             if (esphome::millis() - last_water_full > WATER_FULL_INTERVAL){
//         //                 this->water_full = false;
//         //             }else{
//         //                 this->water_full = true;
//         //             }
//         //             last_water_full = esphome::millis();
//         //         }
//         //         this->water_full_sensor->publish_state(this->water_full);
//         //     }
//         // }
//         // else
//         // {
//         //     if (esphome::millis() - last_adjustment < ADJUSTMENT_INTERVAL)
//         //     {
//         //         continue;
//         //     }
//         //     last_adjustment = esphome::millis();
//         //     // we are adjusting
//         //     if (this->steps_left_to_adjust_temp > 0)
//         //     {
//         //         if (this->target_temperature != packet.get_temp())
//         //         {
//         //             auto packet_to_send = BUTTON_LOWER_TEMP;
//         //             if (this->target_temperature > packet.get_temp())
//         //             {
//         //                 packet_to_send = BUTTON_HIGHER_TEMP;
//         //                 ESP_LOGD(TAG, "Sending BUTTON_HIGHER_TEMP packet to AC");
//         //             }
//         //             else
//         //             {
//         //                 ESP_LOGD(TAG, "Sending BUTTON_LOWER_TEMP packet to AC");
//         //             }
//         //             // create a vector from BUTTON_UP, which is an std::array
//         //             std::vector<uint8_t> packet_vector(packet_to_send.begin(), packet_to_send.end());

//         //             this->send_rmt_data(this->rmt_ac_tx, packet_vector);
//         //             this->steps_left_to_adjust_temp--;
//         //         }
//         //         else
//         //         {
//         //             this->steps_left_to_adjust_temp = 0;
//         //         }
//         //     }
//         //     if (this->steps_left_to_adjust_fan > 0)
//         //     {
//         //         if (this->fan_mode != fan_from_packet)
//         //         {
//         //             auto packet_to_send = BUTTON_FAN;

//         //             // create a vector from BUTTON_FAN, which is an std::array
//         //             std::vector<uint8_t> packet_vector(packet_to_send.begin(), packet_to_send.end());
//         //             ESP_LOGD(TAG, "Sending BUTTON_FAN packet to AC");
//         //             this->send_rmt_data(this->rmt_ac_tx, packet_vector);
//         //             this->steps_left_to_adjust_fan--;
//         //         }
//         //         else
//         //         {
//         //             this->steps_left_to_adjust_fan = 0;
//         //         }
//         //     }
//         //     if (this->preset != preset_from_packet)
//         //     {
//         //         auto packet_to_send = BUTTON_SLEEP;
//         //         // create a vector from BUTTON_SLEEP, which is an std::array
//         //         std::vector<uint8_t> packet_vector(packet_to_send.begin(), packet_to_send.end());
//         //         ESP_LOGD(TAG, "Sending BUTTON_SLEEP packet to AC");
//         //         this->send_rmt_data(this->rmt_ac_tx, packet_vector);
//         //         this->adjust_preset = false;
//         //     }
//         //     if (this->steps_left_to_adjust_mode > 0)
//         //     {
//         //         if (this->mode != mode_from_packet)
//         //         {
//         //             auto packet_to_send = BUTTON_MODE;
//         //             if (this->mode == esphome::climate::ClimateMode::CLIMATE_MODE_OFF || mode_from_packet == esphome::climate::CLIMATE_MODE_OFF)
//         //             {
//         //                 packet_to_send = BUTTON_ON;
//         //                 ESP_LOGD(TAG, "Sending BUTTON_ON packet to AC");
//         //             }
//         //             else
//         //             {
//         //                 ESP_LOGD(TAG, "Sending BUTTON_MODE packet to AC");
//         //             }
//         //             // create a vector from BUTTON_MODE, which is an std::array
//         //             std::vector<uint8_t> packet_vector(packet_to_send.begin(), packet_to_send.end());

//         //             this->send_rmt_data(this->rmt_ac_tx, packet_vector);
//         //             this->steps_left_to_adjust_mode--;
//         //         }
//         //         else
//         //         {
//         //             this->steps_left_to_adjust_mode = 0;
//         //         }
//         //     }

//         // }
//         // this->send_rmt_data(this->rmt_panel_tx, packet.to_wire_format());
//     }
// }



}  // namespace JHS
