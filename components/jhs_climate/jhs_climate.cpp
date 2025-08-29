#include "jhs_climate.h"

static const char *TAG = "JHSClimate";

namespace JHS {

JHSClimate::JHSClimate(int panel_tx_pin, int panel_rx_pin, uart_port_t uart_num)
    : tx_pin_(panel_tx_pin), rx_pin_(panel_rx_pin), uart_num_(uart_num) {}

void JHSClimate::setup() {
  ESP_LOGI(TAG, "JHSClimate initialized on TX=%d RX=%d", tx_pin_, rx_pin_);

  uart_config_t uart_config = {
      .baud_rate = 9600,
      .data_bits = UART_DATA_8_BITS,
      .parity = UART_PARITY_DISABLE,
      .stop_bits = UART_STOP_BITS_1,
      .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
  };

  uart_param_config(uart_num_, &uart_config);
  uart_set_pin(uart_num_, tx_pin_, rx_pin_, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
  uart_driver_install(uart_num_, 512, 0, 0, NULL, 0);
}


// void JHSClimate::loop() {
//   uint8_t buf[128];
//   int len = uart_read_bytes(uart_num_, buf, sizeof(buf), 10 / portTICK_PERIOD_MS);
//   if (len > 0) {
//     for (int i = 0; i < len; i++) {
//       ESP_LOGI(TAG, "UART received: 0x%02X ('%c')", buf[i], (buf[i] >= 32 && buf[i] <= 126) ? buf[i] : '.');
//     }
//   }
// }

void JHSClimate::loop() {
  uint8_t b;
  int len = uart_read_bytes(uart_num_, &b, 1, 0);
  if (len > 0) {
    handle_uart_byte(b);
  }
}

void JHSClimate::handle_uart_byte(uint8_t b) {
  buffer_.push_back(b);

  // Prüfen, ob wir ein komplettes Paket haben
  if (buffer_.size() == sizeof(JHSAcPacket) + 1) {
    JHSAcPacket pkt = JHSAcPacket::parse(buffer_);
    ESP_LOGI("JHSClimate", "Packet: %s", pkt.to_string().c_str());
    buffer_.clear();
  }
}


}  // namespace JHS
