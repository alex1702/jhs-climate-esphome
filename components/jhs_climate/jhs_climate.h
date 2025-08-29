#pragma once
#include "esphome.h"
#include "jhs_packets.h"

namespace JHS {

class JHSClimate : public PollingComponent, public uart::UARTDevice {
    public:
    explicit JHSClimate(esphome::uart::UARTComponent *parent) : UARTDevice(parent) {}

    void setup() override;
    void update() override;

    private:
    std::vector<uint8_t> buffer_;
};

}  // namespace JHS
