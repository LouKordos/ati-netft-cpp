#ifndef NETFT_SENSOR
#define NETFT_SENSOR

#include <string>
#include <array>

struct force {
    double x;
    double y;
    double z;
};

struct torque {
    double x;
    double y;
    double z;
};

class netft_sensor {
    private:
        std::string ip;
        int port;
        std::array<double, 3> force_zero_offset{0, 0, 0};
        std::array<double, 3> torque_zero_offset{0, 0, 0};
        force current_force {};
        torque current_torque {};

    public:
        netft_sensor::netft_sensor(std::string ip, int port = 49151) : ip(ip), port(port) {

        }
        void zero_sensor() {
            sensor_data = this->get_data();
            force_zero_offset = sensor_data.force;
            torque_zero_offset = sensor_data.torque;
        }
        std::pair<double[3], double[3]> get_zero_offset() {

        }
        
        bool connect() {

        }

        
};

#endif