#pragma once
#include "esphome.h"
//#include "driver/uart.h"
#include "jhs_packets.h"
//#include "esphome/core/component.h"
// #include "esphome/components/gpio/gpio.h"
#include "esphome/core/gpio.h"

namespace JHS {

class JHSClimate : public esphome::Component
{
public:
    //JHSClimate(int panel_tx_pin, int panel_rx_pin, uart_port_t uart_num = UART_NUM_1); // old
    // pin setters
    void set_ac_tx_pin(esphome::InternalGPIOPin *ac_tx_pin) { ac_tx_pin_ = ac_tx_pin; }
    void set_ac_rx_pin(esphome::InternalGPIOPin *ac_rx_pin) { ac_rx_pin_ = ac_rx_pin; }
    void set_panel_tx_pin(esphome::InternalGPIOPin *panel_tx_pin) { panel_tx_pin_ = panel_tx_pin; }
    void set_panel_rx_pin(esphome::InternalGPIOPin *panel_rx_pin) { panel_rx_pin_ = panel_rx_pin; }

    //void set_water_full_sensor(esphome::binary_sensor::BinarySensor *water_full_sensor_) { water_full_sensor  = water_full_sensor_; }

    void setup() override;
    // esphome::climate::ClimateTraits traits() override;
    // void control(const esphome::climate::ClimateCall &call) override;
    void dump_config() override;
    void loop();

protected:
    esphome::InternalGPIOPin *ac_tx_pin_;
    esphome::InternalGPIOPin *ac_rx_pin_;
    esphome::InternalGPIOPin *panel_tx_pin_;
    esphome::InternalGPIOPin *panel_rx_pin_;
    // esphome::binary_sensor::BinarySensor *water_full_sensor;

private:

    void recv_from_panel();

    void recv_from_ac();
};

}  // namespace JHS
