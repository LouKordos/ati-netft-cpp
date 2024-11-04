#ifndef NETFT_SENSOR
#define NETFT_SENSOR

#include <string>
#include <array>
#include <expected>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>

struct vec3d {
    double x;
    double y;
    double z;
};

class netft_sensor {
    private:
        std::string ip;
        int port;
        double timeout_sec;
        vec3d force_zero_offset{0, 0, 0};
        vec3d torque_zero_offset{0, 0, 0};
        vec3d current_force {0, 0, 0};
        vec3d current_torque {0, 0, 0};
        bool connected {false};
        int socket_fd;

    public:
        netft_sensor(const std::string ip, const int port = 49151, const double timeout_sec = 0.01) : ip(ip), port(port), timeout_sec(timeout_sec) {
            init();
        }

        ~netft_sensor() {
            if(connected) {
                close(socket_fd);
                connected = false;
            }
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
        
        bool init() {
            socket_fd = socket(AF_INET, SOCK_STREAM, 0);

            if(socket_fd == -1) {
                connected = false;
            }

            sockaddr_in server_addr;
            server_addr.sin_addr.s_addr = inet_addr(ip.c_str());
            server_addr.sin_port = htons(port);
            server_addr.sin_family = AF_INET;

            if(connect(socket_fd, (sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
                connected = false;
            }
            else {
                connected = true;
            }

            timeval timeout;
            timeout.tv_sec = 0;
            timeout.tv_usec = timeout_sec * 1e+6;
            setsockopt(socket_fd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

            return connected;
        }

        std::expected<std::pair<vec3d, vec3d>, std::string> get_data() {
            std::pair<vec3d, vec3d> data {};
            if(!connected) {
                return std::unexpected("Not connected yet.");
            }

            char buffer[1024];
            ssize_t bytes_received = recv(socket_fd, buffer, sizeof(buffer), 0);

            if(bytes_received == -1) {
                if(errno == EWOULDBLOCK || errno == EAGAIN) {
                    return std::unexpected("Timeout occured while reading Force / Torque data.");
                }
                else {
                    return std::unexpected("An error occured while reading Force / Torque data.");
                }
            }
            else if (bytes_received == 0) {
                connected = false;
                return std::unexpected("Connection closed by the server.");
            }

            return data;
        }
};

#endif