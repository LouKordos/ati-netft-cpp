#include <string>
#include <iostream>
#include "../include/netft_sensor.hpp"
#include <print>

int main() {
    netft_sensor* force_sensor = new netft_sensor("10.36.12.7");
    bool success = force_sensor->zero_sensor();
    std::print("{0}\n", success);
    std::print("{0}\n", force_sensor->get_zero_offset().first.x);
    std::pair<vec3d, vec3d> sensor_data = force_sensor->get_data();

    std::print("Force Z: {0}\n", sensor_data.first.z);
    return 0;
}