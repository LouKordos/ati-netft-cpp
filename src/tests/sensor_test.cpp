#include <string>
#include <iostream>
#include "../include/netft_sensor.hpp"
#include <print>
#include <expected>
#include <tracy/Tracy.hpp>

int main() {
    ZoneScoped;
    ati_netft::netft_sensor force_sensor("10.36.12.7");
    bool connected = force_sensor.initialize_connection();
    bool calibrated = force_sensor.calibrate();
    // bool success = force_sensor->zero_sensor();
    // std::print("connected={0}\tzero_success={1}\tcalibrated={2}\n", connected, success, calibrated);
    std::println("force_zero_offset={0}, {1}, {2}\ttorque_zero_offset={3}, {4}, {5}", force_sensor.get_zero_offset().first[0], force_sensor.get_zero_offset().first[1], force_sensor.get_zero_offset().first[2], force_sensor.get_zero_offset().second[0], force_sensor.get_zero_offset().second[1], force_sensor.get_zero_offset().second[2]);

    for(int i = 0; i < 10000; i++) {
        FrameMark;
        // ZoneScopedN("Get data");
        std::expected<std::pair<std::array<double, 3>, std::array<double, 3>>, std::string> sensor_data = force_sensor.get_data();
        ZoneNamedN(aftergetdatazone, "Before has_value", true);
        if(sensor_data.has_value()) {
            ZoneNamedN(afterhasvaluecheckzone, "After has_value check", true);
            auto [forces, torques] = sensor_data.value();
            std::println("Force [N]: {0:.4f} {1:.4f} {2:.4f}\tTorque [Nm]: {3:.4f} {4:.4f} {5:.4f}", forces[0], forces[1], forces[2], torques[0], torques[1], torques[2]);
        }
        else {
            std::println("Error while getting force / torque sensor data: {0}", sensor_data.error());
            exit(1);
        }
    }
    return 0;
}