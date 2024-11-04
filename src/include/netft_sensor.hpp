#ifndef NETFT_SENSOR
#define NETFT_SENSOR

#include <string>
#include <array>
#include <expected>

struct vec3d {
    double x;
    double y;
    double z;
};

class netft_sensor {
    private:
        std::string ip;
        int port;
        vec3d force_zero_offset{0, 0, 0};
        vec3d torque_zero_offset{0, 0, 0};
        vec3d current_force {};
        vec3d current_torque {};

    public:
        netft_sensor::netft_sensor(const std::string ip, const int port = 49151) : ip(ip), port(port) {

        }
        void zero_sensor() {
            // sensor_data = this->get_data();
            force_zero_offset = sensor_data.force;
            torque_zero_offset = sensor_data.torque;
        }

        std::pair<vec3d, vec3d> get_zero_offset() {
            return std::pair<vec3d, vec3d>{force_zero_offset, torque_zero_offset};
        }
        
        bool connect() {

        }
};

#endif