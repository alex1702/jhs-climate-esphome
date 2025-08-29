#pragma once
#include "esphome.h"
#include "esphome/components/uart/uart_component.h"

namespace JHS {

class JHSClimate : public PollingComponent {
public:
  explicit JHSClimate(esphome::uart::UARTComponent *uart) : uart_(uart) {}

  void setup() override;
  void update() override;

private:
  esphome::uart::UARTComponent *uart_;
};

}  // namespace JHS
