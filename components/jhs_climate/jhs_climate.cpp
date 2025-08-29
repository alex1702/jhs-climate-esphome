#include "jhs_climate.h"

static const char *TAG = "JHSClimate";

namespace JHS {

void JHSClimate::setup() {
  ESP_LOGI(TAG, "JHSClimate component initialized");
}

void JHSClimate::update() {
  if (!uart_) return;

  uint8_t b;
  while (uart_->available()) {
    if (uart_->read_byte(&b)) {
      ESP_LOGI(TAG, "UART received: 0x%02X ('%c')", b, (b >= 32 && b <= 126) ? b : '.');
    }
  }
}

}  // namespace JHS
