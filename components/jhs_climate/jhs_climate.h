#pragma once
#include "esphome.h"
#include "driver/uart.h"

namespace JHS {

class JHSClimate : public esphome::Component
{
public:
  JHSClimate(int panel_tx_pin, int panel_rx_pin, uart_port_t uart_num = UART_NUM_1);

  void setup() override;
  void loop();

private:
  int tx_pin_;
  int rx_pin_;
  uart_port_t uart_num_;
};

}  // namespace JHS
