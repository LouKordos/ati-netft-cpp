#include "include/netft_sensor.hpp"

netft_sensor::netft_sensor(std::string ip, int port) {
    this->ip  = ip;
    this->port = port;
}