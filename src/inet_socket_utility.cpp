#include "inet_socket_utility.hpp"

#include <arpa/inet.h>

auto tristan::sockets::utility::uint32_tIpToStringIp(uint32_t ip) -> std::string {
    char buf[INET_ADDRSTRLEN];
    std::string address = inet_ntop(AF_INET, &ip, buf, sizeof(buf));
    if (address.empty()){
        return {};
    }
    return address;
}

auto tristan::sockets::utility::stringIpToUint32_tIp(const std::string& ip) -> uint32_t {
    uint32_t result = 0;
    if (inet_pton(AF_INET, ip.c_str(), &result) <= 0){
        return {};
    }
    return result;
}

auto tristan::sockets::utility::portValueLocalToNet(uint16_t port) -> uint16_t {
    return htons(port);
}

auto tristan::sockets::utility::portValueNetToLocal(uint16_t port) -> uint16_t {
    return ntohs(port);
}
