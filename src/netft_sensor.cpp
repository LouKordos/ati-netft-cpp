#include "include/netft_sensor.hpp"

netft_sensor::netft_sensor(std::string ip, int port) : ip(ip), port(port) {
    for(std::size_t i = 0; i < this->F_zero_offset.size(); i++) {
        this->F_zero_offset[i] = 0;
        this->T_zero_offset[i] = 0;
    }
}