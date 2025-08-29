#pragma once
#include "esphome.h"

namespace JHS {

class JHSClimate : public PollingComponent, public uart::UARTDevice {
public:
  explicit JHSClimate(esphome::uart::UARTComponent *parent) : UARTDevice(parent) {}

  void setup() override;
  void update() override;
};

}  // namespace JHS
