#include "esphome.h"
#include "driver/uart.h"

namespace JHS {

class JHSClimate : public Component {  // <-- Component erben
public:
  JHSClimate(int panel_tx_pin, int panel_rx_pin, int uart_num = UART_NUM1);

  void setup() override;   // override hinzufügen
  void loop();

private:
  int tx_pin_;
  int rx_pin_;
  uart_port_t uart_num_;
};

}  // namespace JHS
