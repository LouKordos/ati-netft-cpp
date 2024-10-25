#ifndef NETFT_SENSOR
#define NETFT_SENSOR

#include <string>
#include <array>

class netft_sensor {
    private: 
        std::string ip;
        int port;
        std::array<double, 3> F_zero_offset;
        std::array<double, 3> T_zero_offset;

    public:
        netft_sensor(std::string ip, int port = 49151);
        void zero_sensor();
        std::pair<double[3], double[3]> get_zero_offset();
        bool connect();
};

#endif