#include <string>
#include <iostream>
#include "../include/netft_sensor.hpp"
#include <print>
#include <expected>
#include <tracy/Tracy.hpp>


int main() {
    ZoneScoped;
    netft_sensor* force_sensor = new netft_sensor("10.36.12.7");
    bool connected = force_sensor->init();
    bool calibrated = force_sensor->calibrate();
    // bool success = force_sensor->zero_sensor();
    // std::print("connected={0}\tzero_success={1}\tcalibrated={2}\n", connected, success, calibrated);
    std::print("{0}\n", force_sensor->get_zero_offset().first.x);

    for(int i = 0; i < 10000; i++) {
        FrameMark;
        // ZoneScopedN("Get data");
        std::expected<std::pair<vec3d, vec3d>, std::string> sensor_data = force_sensor->get_data();
        ZoneNamedN(aftergetdatazone, "Before has_value", true);
        if(sensor_data.has_value()) {
            ZoneNamedN(afterhasvaluecheckzone, "After has_value check", true);
            // ZoneScopedN("Has Value");
            std::print("Force Z: {0}\n", sensor_data.value().first.z);
        }
        else {
            std::print("Error while getting force / torque sensor data: {0}\n", sensor_data.error());
        }
    }
    return 0;
}