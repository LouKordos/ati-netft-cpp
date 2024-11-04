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
        vec3d current_force {0, 0, 0};
        vec3d current_torque {0, 0, 0};

    public:
        netft_sensor(const std::string ip, const int port = 49151) : ip(ip), port(port) {

        }

        bool zero_sensor() {
            std::expected<std::pair<vec3d, vec3d>, std::string> sensor_data = this->get_data();
            if(sensor_data.has_value()) {
                force_zero_offset = sensor_data.value().first;
                torque_zero_offset = sensor_data.value().second;

                return true;
            }
            else  {
                return false;
            }
        }

        std::pair<vec3d, vec3d> get_zero_offset() {
            return std::pair<vec3d, vec3d>{force_zero_offset, torque_zero_offset};
        }
        
        bool connect() {
            
        }

        std::expected<std::pair<vec3d, vec3d>, std::string> get_data() {
            std::pair<vec3d, vec3d> data {};
            return data;
        }
};

#endif