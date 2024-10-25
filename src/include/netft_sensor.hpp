#ifndef NETFT_SENSOR
#define NETFT_SENSOR

#include <string>

class netft_sensor {
    private: 
        std::string ip;
        int port;

    public:
        netft_sensor(std::string ip, int port = 49151);
        void zero_sensor();
        void get_zero_offset()
    
};

#endif