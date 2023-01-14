#ifndef SOCKETS_INET_SOCKET_UTILITY_HPP
#define SOCKETS_INET_SOCKET_UTILITY_HPP

#include <cstdint>
#include <string>

namespace tristan::sockets::utility {

    [[nodiscard]] auto uint32_tIpToStringIp(uint32_t ip) -> std::string;

    [[nodiscard]] auto stringIpToUint32_tIp(const std::string& ip) -> uint32_t;

    [[nodiscard]] auto portValueLocalToNet(uint16_t port) -> uint16_t;

    [[nodiscard]] auto portValueNetToLocal(uint16_t port) -> uint16_t;

} //End of tristan::sockets::utility namespace

#endif  //SOCKETS_INET_SOCKET_UTILITY_HPP
