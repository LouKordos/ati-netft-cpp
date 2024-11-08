#ifndef NETFT_SENSOR
#define NETFT_SENSOR

#include <string>
#include <array>
#include <expected>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <span>
#include <cstdint>
#include <cerrno>
#include <cstring>
#include <tracy/Tracy.hpp>

struct vec3d {
    double x;
    double y;
    double z;
};

class netft_sensor {

    struct message {
        uint8_t command;
        uint8_t reserved[19];
    };
    // https://www.ati-ia.com/app_content/documents/9620-05-NET%20FT.pdf
    struct calibration_info {
        uint16_t header;
        uint8_t force_units;
        uint8_t torque_units;
        uint32_t countsPerForce;
        uint32_t countsPerTorque;
        uint16_t scaleFactors[6];
    };

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
        calibration_info calibration_data;

    public:
        netft_sensor(const std::string ip, const int port = 49151, const double timeout_sec = 0.01) : ip(ip), port(port), timeout_sec(timeout_sec) {

        }

        ~netft_sensor() {
            if(connected) {
                close(socket_fd);
                connected = false;
            }
        }

        bool zero_sensor() {
            std::expected<std::pair<vec3d, vec3d>, std::string> sensor_data = this->get_data(true);
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

        bool calibrate() {
            ZoneScoped;
            if(!connected) {
                return false;
            }

            message get_cal_info_msg {}; // See https://www.ati-ia.com/app_content/documents/9620-05-NET%20FT.pdf
            get_cal_info_msg.command = 1;

            if(send(socket_fd, &get_cal_info_msg, sizeof(get_cal_info_msg), 0) == -1) {
                return false;
            }

            uint8_t buffer[1024];
            ssize_t bytes_received = recv(socket_fd, buffer, sizeof(buffer), 0);

            size_t expected_size = sizeof(uint16_t) * 2 + sizeof(uint8_t) * 2 + sizeof(uint32_t) * 2 + sizeof(uint16_t) * 2;

            if(bytes_received == -1 || static_cast<size_t>(bytes_received) < expected_size) {
                return false;
            }
            // Connection closed
            else if (bytes_received == 0) {
                connected = false;
                return false;
            }

            std::span<uint8_t> buffer_span(buffer, bytes_received);
            size_t offset = 0;

            uint16_t header_net;
            std::memcpy(&header_net, &buffer_span[offset], sizeof(header_net));
            calibration_data.header = ntohs(header_net);
            offset += sizeof(header_net);

            calibration_data.force_units = buffer_span[offset];
            offset += sizeof(uint8_t);
            calibration_data.torque_units = buffer_span[offset];
            offset += sizeof(uint8_t);

            uint32_t counts_per_force_net;
            std::memcpy(&counts_per_force_net, &buffer_span[offset], sizeof(counts_per_force_net));
            calibration_data.countsPerForce = ntohl(counts_per_force_net);
            offset += sizeof(counts_per_force_net);

            uint32_t counts_per_torque_net;
            std::memcpy(&counts_per_torque_net, &buffer_span[offset], sizeof(counts_per_torque_net));
            calibration_data.countsPerTorque = ntohl(counts_per_torque_net);
            offset += sizeof(counts_per_torque_net);

            for(int i = 0; i < 6; i++) {
                uint16_t scale_factor_net;
                std::memcpy(&scale_factor_net, &buffer_span[offset], sizeof(scale_factor_net));
                calibration_data.scaleFactors[i] = ntohs(scale_factor_net);
                offset += sizeof(scale_factor_net);
            }

            return true;
        }

        std::expected<std::pair<vec3d, vec3d>, std::string> get_data(bool without_zero_offset = false) {
            ZoneScoped;
            if(!connected) {
                return std::unexpected("Not connected yet.");
            }

            message get_data_msg {}; // See https://www.ati-ia.com/app_content/documents/9620-05-NET%20FT.pdf
            {
                ZoneScopedN("Send READFT msg");
                if(send(socket_fd, &get_data_msg, sizeof(get_data_msg), 0) == -1) {
                    return std::unexpected("get_data_msg send failed.");
                }
            }

            uint8_t buffer[1024];
            ZoneNamedN(recvzone, "Recv", true);
            ssize_t bytes_received = recv(socket_fd, buffer, sizeof(buffer), 0);
            ZoneNamedN(lengthcheckzone, "Length check", true);
            size_t expected_size = sizeof(uint16_t) * 2 + sizeof(int16_t) * 6;

            if(bytes_received == -1) {
                if(errno == EWOULDBLOCK || errno == EAGAIN) {
                    return std::unexpected("Timeout occured while reading Force / Torque data.");
                }
                else {
                    return std::unexpected("An error occured while reading Force / Torque data.");
                }
            }
            else if(bytes_received == 0) {
                connected = false;
                return std::unexpected("Connection closed by the server.");
            }
            else if(static_cast<size_t>(bytes_received) < expected_size) {
                return std::unexpected(std::format("Not enough bytes received. bytes_received={0}, expected_bytes={1}", bytes_received, expected_size));
            }
            ZoneNamedN(parsezone, "Parse", true);
            std::span<uint8_t> buffer_span(buffer, bytes_received);
            size_t offset = sizeof(uint16_t) + sizeof(uint16_t); // Skip header and status

            // FORCES
            uint16_t force_x_net;
            std::memcpy(&force_x_net, &buffer_span[offset], sizeof(force_x_net));
            offset += sizeof(force_x_net);
            // noths returns uint16_t but the actual value is int16, so we need two casts
            current_force.x = static_cast<double>(static_cast<int16_t>(ntohs(force_x_net))) * static_cast<double>(calibration_data.scaleFactors[0]) / static_cast<double>(calibration_data.countsPerForce);
            if(!without_zero_offset) {
                current_force.x -= force_zero_offset.x;
            }

            int16_t force_y_net;
            std::memcpy(&force_y_net, &buffer_span[offset], sizeof(force_y_net));
            offset += sizeof(force_y_net);

            current_force.y = static_cast<double>(static_cast<int16_t>(ntohs(force_y_net))) * static_cast<double>(calibration_data.scaleFactors[1]) / static_cast<double>(calibration_data.countsPerForce);
            if(!without_zero_offset) {
                current_force.y -= force_zero_offset.y;
            }

            int16_t force_z_net;
            std::memcpy(&force_z_net, &buffer_span[offset], sizeof(force_z_net));
            offset += sizeof(force_z_net);

            current_force.z = static_cast<double>(static_cast<int16_t>(ntohs(force_z_net))) * static_cast<double>(calibration_data.scaleFactors[2]) / static_cast<double>(calibration_data.countsPerForce);
            if(!without_zero_offset) {
                current_force.z -= force_zero_offset.z;
            }

            return std::pair<vec3d, vec3d>{current_force, current_torque};
        }
};

#endif