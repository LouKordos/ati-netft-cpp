#include <string>
#include <iostream>
#include "../include/netft_sensor.hpp"
#include <print>

int main() {
    netft_sensor* force_sensor = new netft_sensor("10.36.12.7");
    bool success = force_sensor->zero_sensor();
    std::print("{0}", success)
    return 0;
}