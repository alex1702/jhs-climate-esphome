#pragma once


#include "esphome.h"
#include "jhs_packets.h"


namespace JHS {


    class JHSClimate : public esphome::Component, public esphome::uart::UARTDevice {
        public:
        explicit JHSClimate(esphome::uart::UARTComponent *parent) : UARTDevice(parent) {}


        void setup() override;
        void loop() override;
        void dump_config() override;


        private:
        std::vector<uint8_t> buffer_;
    };


} // namespace JHS