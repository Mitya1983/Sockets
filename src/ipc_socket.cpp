#include "ipc_socket.hpp"
#include "socket_error.hpp"

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/fcntl.h>
#include <sys/un.h>
#include <unistd.h>

tristan::sockets::IpcSocket::IpcSocket(SocketType socket_type) :
    m_socket(-1),
    m_type(socket_type),
    m_global_namespace(false),
    m_peer_global_namespace(false),
    m_non_blocking(false),
    m_bound(false),
    m_listening(false),
    m_connected(false) {
    if (m_type == tristan::sockets::SocketType::STREAM) {
        m_socket = socket(AF_UNIX, SOCK_STREAM, 0);
    } else {
        m_socket = socket(AF_UNIX, SOCK_DGRAM, 0);
    }
    if (m_socket < 0) {
        tristan::sockets::Error error{};
        switch (errno) {
            case EPROTONOSUPPORT: {
                error = tristan::sockets::Error::SOCKET_PROTOCOL_NOT_SUPPORTED;
                break;
            }
            case EMFILE: {
                error = tristan::sockets::Error::SOCKET_PROCESS_TABLE_IS_FULL;
                break;
            }
            case ENFILE: {
                error = tristan::sockets::Error::SOCKET_SYSTEM_TABLE_IS_FULL;
                break;
            }
            case EACCES: {
                error = tristan::sockets::Error::SOCKET_NOT_ENOUGH_PERMISSIONS;
                break;
            }
            case ENOSR: {
                error = tristan::sockets::Error::SOCKET_NOT_ENOUGH_MEMORY;
                break;
            }
            case EPROTOTYPE: {
                error = tristan::sockets::Error::SOCKET_WRONG_PROTOCOL;
                break;
            }
        }
        m_error = tristan::sockets::makeError(error);
    }
}

tristan::sockets::IpcSocket::~IpcSocket() { IpcSocket::close(); }

void tristan::sockets::IpcSocket::setName(const std::string& name, bool global_namespace) {
    m_global_namespace = global_namespace;
    if (m_global_namespace) {
        m_name = "#";
    }
    m_name += name;
}

void tristan::sockets::IpcSocket::setPeerName(const std::string& name, bool global_namespace) {
    m_peer_global_namespace = global_namespace;
    if (m_peer_global_namespace) {
        m_peer_name = "#";
    }
    m_peer_name += name;
}

void tristan::sockets::IpcSocket::setNonBlocking(bool non_blocking) {
    if (m_socket == -1) {
        m_error = tristan::sockets::makeError(tristan::sockets::Error::SOCKET_NOT_INITIALISED);
        return;
    }
    int32_t status;
    if (non_blocking) {
        status = fcntl(m_socket, F_SETFL, O_NONBLOCK);
    } else {
        status = fcntl(m_socket, F_SETFL, 0);
    }
    if (status < 0) {
        m_error = tristan::sockets::makeError(tristan::sockets::Error::SOCKET_FCNTL_ERROR);
        return;
    }
    m_non_blocking = non_blocking;
}

void tristan::sockets::IpcSocket::resetError() { m_error = tristan::sockets::makeError(tristan::sockets::Error::SUCCESS); }

void tristan::sockets::IpcSocket::bind() {
    if (m_socket == -1) {
        m_error = tristan::sockets::makeError(tristan::sockets::Error::SOCKET_NOT_INITIALISED);
        return;
    }
    if (m_bound) {
        return;
    }
    sockaddr_un address{};
    address.sun_family = AF_UNIX;
    strcpy(address.sun_path, m_name.c_str());
    if (m_name.at(0) == '#'){
        address.sun_path[0] = 0;
    }
    auto address_length = sizeof(address.sun_family) + m_name.size();
    auto status = ::bind(m_socket, reinterpret_cast< const struct sockaddr* >(&address), address_length);
    if (status < 0) {
        tristan::sockets::Error error{};
        switch (errno) {
            case EACCES: {
                error = tristan::sockets::Error::BIND_NOT_ENOUGH_PERMISSIONS;
                break;
            }
            case EADDRINUSE: {
                error = tristan::sockets::Error::BIND_ADDRESS_IN_USE;
                break;
            }
            case EBADF: {
                error = tristan::sockets::Error::BIND_BAD_FILE_DESCRIPTOR;
                break;
            }
            case EINVAL: {
                error = tristan::sockets::Error::BIND_ALREADY_BOUND;
                break;
            }
            case ENOTSOCK: {
                error = tristan::sockets::Error::BIND_FILE_DESCRIPTOR_IS_NOT_SOCKET;
                break;
            }
            case EADDRNOTAVAIL: {
                error = tristan::sockets::Error::BIND_ADDRESS_NOT_AVAILABLE;
                break;
            }
            case EFAULT: {
                error = tristan::sockets::Error::BIND_ADDRESS_OUTSIDE_USER_SPACE;
                break;
            }
            case ELOOP: {
                error = tristan::sockets::Error::BIND_TO_MANY_SYMBOLIC_LINKS;
                break;
            }
            case ENAMETOOLONG: {
                error = tristan::sockets::Error::BIND_NAME_TO_LONG;
                break;
            }
            case ENOENT: {
                error = tristan::sockets::Error::BIND_NO_ENTRY;
                break;
            }
            case ENOMEM: {
                error = tristan::sockets::Error::BIND_NO_MEMORY;
                break;
            }
            case ENOTDIR: {
                error = tristan::sockets::Error::BIND_NOT_DIRECTORY;
                break;
            }
            case EROFS: {
                error = tristan::sockets::Error::BIND_READ_ONLY_FS;
                break;
            }
        }
        m_error = tristan::sockets::makeError(error);
    }
    if (not m_error) {
        m_bound = true;
    }
}

void tristan::sockets::IpcSocket::listen(uint32_t connection_count_limit) {
    if (m_socket == -1) {
        m_error = tristan::sockets::makeError(tristan::sockets::Error::SOCKET_NOT_INITIALISED);
        return;
    }
    if (m_connected) {
        m_error = tristan::sockets::makeError(tristan::sockets::Error::LISTEN_ALREADY_CONNECTED);
        return;
    }
    if (not m_bound) {
        m_error = tristan::sockets::makeError(tristan::sockets::Error::LISTEN_NOT_BOUND);
        return;
    }
    auto status = ::listen(m_socket, static_cast< int32_t >(connection_count_limit));
    if (status < 0) {
        tristan::sockets::Error error{};
        switch (errno) {
            case EADDRINUSE: {
                error = tristan::sockets::Error::LISTEN_ADDRESS_IN_USE;
                break;
            }
            case EBADF: {
                error = tristan::sockets::Error::LISTEN_BAD_FILE_DESCRIPTOR;
                break;
            }
            case ENOTSOCK: {
                error = tristan::sockets::Error::LISTEN_FILE_DESCRIPTOR_IS_NOT_SOCKET;
                break;
            }
            case EOPNOTSUPP: {
                error = tristan::sockets::Error::LISTEN_PROTOCOL_NOT_SUPPORTED;
                break;
            }
        }
        m_error = tristan::sockets::makeError(error);
    }
    m_listening = true;
}

void tristan::sockets::IpcSocket::connect() {
    if (m_socket == -1) {
        m_error = tristan::sockets::makeError(tristan::sockets::Error::SOCKET_NOT_INITIALISED);
        return;
    }
    if (m_listening) {
        m_error = tristan::sockets::makeError(tristan::sockets::Error::CONNECT_SOCKET_IS_IN_LISTEN_MODE);
        return;
    }
    if (not m_connected) {
        sockaddr_un peer_address{};
        peer_address.sun_family = AF_UNIX;
        strcpy(peer_address.sun_path, m_peer_name.c_str());
        if (m_peer_name.at(0) == '#'){
            peer_address.sun_path[0] = 0;
        }
        auto address_length = sizeof(peer_address.sun_family) + m_peer_name.size();
        auto status = ::connect(m_socket, reinterpret_cast< struct sockaddr* >(&peer_address), address_length);
        if (status < 0) {
            tristan::sockets::Error error{};
            switch (errno) {
                case EACCES: {
                    [[fallthrough]];
                }
                case EPERM: {
                    error = tristan::sockets::Error::CONNECT_NOT_ENOUGH_PERMISSIONS;
                    break;
                }
                case EADDRINUSE: {
                    error = tristan::sockets::Error::CONNECT_ADDRESS_IN_USE;
                    break;
                }
                case EADDRNOTAVAIL: {
                    error = tristan::sockets::Error::CONNECT_ADDRESS_NOT_AVAILABLE;
                    break;
                }
                case EAFNOSUPPORT: {
                    error = tristan::sockets::Error::CONNECT_AF_NOT_SUPPORTED;
                    break;
                }
                case EAGAIN: {
                    error = tristan::sockets::Error::CONNECT_TRY_AGAIN;
                    break;
                }
                case EALREADY: {
                    error = tristan::sockets::Error::CONNECT_ALREADY_IN_PROCESS;
                    break;
                }
                case EBADF: {
                    error = tristan::sockets::Error::CONNECT_BAD_FILE_DESCRIPTOR;
                    break;
                }
                case ECONNREFUSED: {
                    error = tristan::sockets::Error::CONNECT_CONNECTION_REFUSED;
                    break;
                }
                case EFAULT: {
                    error = tristan::sockets::Error::CONNECT_ADDRESS_OUTSIDE_USER_SPACE;
                    break;
                }
                case EINPROGRESS: {
                    error = tristan::sockets::Error::CONNECT_IN_PROGRESS;
                    break;
                }
                case EINTR: {
                    error = tristan::sockets::Error::CONNECT_INTERRUPTED;
                    break;
                }
                case EISCONN: {
                    error = tristan::sockets::Error::CONNECT_CONNECTED;
                    break;
                }
                case ENETUNREACH: {
                    error = tristan::sockets::Error::CONNECT_NETWORK_UNREACHABLE;
                    break;
                }
                case ENOTSOCK: {
                    error = tristan::sockets::Error::CONNECT_FILE_DESCRIPTOR_IS_NOT_SOCKET;
                    break;
                }
                case EPROTOTYPE: {
                    error = tristan::sockets::Error::CONNECT_PROTOCOL_NOT_SUPPORTED;
                    break;
                }
                case ETIMEDOUT: {
                    error = tristan::sockets::Error::SOCKET_TIMED_OUT;
                    break;
                }
                default:{
                    m_error = std::error_code(errno, std::system_category());
                }
            }
            if (not m_error) {
                m_error = tristan::sockets::makeError(error);
            }
            return;
        }
        m_connected = true;
    }
}

void tristan::sockets::IpcSocket::close() {
    ::close(m_socket);
    ::unlink(m_name.c_str());
}

void tristan::sockets::IpcSocket::shutdown() {
    auto status = ::shutdown(m_socket, SHUT_RDWR);
    if (status < 0){
        tristan::sockets::Error error{};
        switch (errno){
            case EBADF:{
                error = tristan::sockets::Error::SHUTDOWN_INVALID_SOCKET_ARGUMENT;
                break;
            }
            case EINVAL:{
                error = tristan::sockets::Error::SHUTDOWN_INVALID_SHUTDOWN_OPTION_PROVIDED;
                break;
            }
            case ENOTCONN:{
                error = tristan::sockets::Error::SHUTDOWN_NOT_CONNECTED;
                break;
            }
            case ENOTSOCK:{
                error = tristan::sockets::Error::SHUTDOWN_INVALID_FILE_DESCRIPTOR;
                break;
            }
            case ENOBUFS:{
                error = tristan::sockets::Error::SHUTDOWN_NOT_ENOUPH_MEMORY;
                break;
            }
        }
        m_error = tristan::sockets::makeError(error);
    }
}

auto tristan::sockets::IpcSocket::accept() -> std::optional< std::unique_ptr< IpcSocket > > {
    if (m_socket == -1) {
        m_error = tristan::sockets::makeError(tristan::sockets::Error::SOCKET_NOT_INITIALISED);
        return std::nullopt;
    }
    if (not m_listening) {
        m_error = tristan::sockets::makeError(tristan::sockets::Error::ACCEPT_SOCKET_IS_NOT_IN_LISTEN_MODE);
        return std::nullopt;
    }
    if (m_connected) {
        m_error = tristan::sockets::makeError(tristan::sockets::Error::ACCEPT_ALREADY_CONNECTED);
        return std::nullopt;
    }
    if (not m_bound) {
        m_error = tristan::sockets::makeError(tristan::sockets::Error::ACCEPT_NOT_BOUND);
        return std::nullopt;
    }
    sockaddr_un peer_address{};
    uint32_t address_length = sizeof(peer_address);
    std::unique_ptr< tristan::sockets::IpcSocket > socket(new tristan::sockets::IpcSocket(true));
    socket->m_type = m_type;
    socket->m_socket = ::accept(m_socket, reinterpret_cast< struct sockaddr* >(&peer_address), &address_length);
    if (socket->m_socket < 0) {
        tristan::sockets::Error error{};
        switch (errno) {
            case EAGAIN: {
                [[fallthrough]];
            }
            case ENETDOWN: {
                [[fallthrough]];
            }
            case ENOPROTOOPT: {
                [[fallthrough]];
            }
            case EHOSTDOWN: {
                [[fallthrough]];
            }
            case ENONET: {
                [[fallthrough]];
            }
            case EHOSTUNREACH: {
                [[fallthrough]];
            }
            case ENETUNREACH: {
                error = tristan::sockets::Error::ACCEPT_TRY_AGAIN;
                break;
            }
#if defined(EWOULDBLOCK) && EWOULDBLOCK != EAGAIN
            case EWOULDBLOCK: {
                error = tristan::sockets::Error::ACCEPT_TRY_AGAIN;
                break;
            }
#endif
            case EBADF: {
                error = tristan::sockets::Error::ACCEPT_BAD_FILE_DESCRIPTOR;
                break;
            }
            case ECONNABORTED: {
                error = tristan::sockets::Error::ACCEPT_CONNECTION_ABORTED;
                break;
            }
            case EFAULT: {
                error = tristan::sockets::Error::ACCEPT_ADDRESS_OUTSIDE_USER_SPACE;
                break;
            }
            case EINTR: {
                error = tristan::sockets::Error::ACCEPT_INTERRUPTED;
                break;
            }
            case EINVAL: {
                error = tristan::sockets::Error::ACCEPT_INVALID_VALUE;
                break;
            }
            case EMFILE: {
                error = tristan::sockets::Error::ACCEPT_PER_PROCESS_LIMIT_REACHED;
                break;
            }
            case ENFILE: {
                error = tristan::sockets::Error::ACCEPT_SYSTEM_WIDE_LIMIT_REACHED;
                break;
            }
            case ENOBUFS: {
                [[fallthrough]];
            }
            case ENOMEM: {
                error = tristan::sockets::Error::ACCEPT_NOT_ENOUPH_MEMORY;
                break;
            }
            case ENOTSOCK: {
                error = tristan::sockets::Error::ACCEPT_FILE_DESCRIPTOR_IS_NOT_SOCKET;
                break;
            }
            case EPERM: {
                error = tristan::sockets::Error::ACCEPT_FIREWALL;
                break;
            }
            case EOPNOTSUPP: {
                error = tristan::sockets::Error::ACCEPT_OPTION_IS_NOT_SUPPORTED;
                break;
            }
            case EPROTO: {
                error = tristan::sockets::Error::ACCEPT_PROTOCOL_ERROR;
                break;
            }
        }
        m_error = tristan::sockets::makeError(error);
        return std::nullopt;
    }

    if (m_non_blocking) {
        socket->setNonBlocking();
    }
    socket->m_name = m_name;
    socket->m_peer_name = std::string(peer_address.sun_path);
    socket->m_connected = true;
    return socket;
}

auto tristan::sockets::IpcSocket::write(uint8_t byte) -> uint8_t {
    if (m_socket == -1) {
        m_error = tristan::sockets::makeError(tristan::sockets::Error::SOCKET_NOT_INITIALISED);
        return 0;
    }
    if (byte == 0) {
        return 0;
    }
    uint8_t bytes_sent = 0;
    if (m_connected) {
        bytes_sent = ::write(m_socket, &byte, 1);
    } else {
        if (m_type == tristan::sockets::SocketType::STREAM) {
            m_error = tristan::sockets::makeError(tristan::sockets::Error::SOCKET_NOT_CONNECTED);
        } else {
            sockaddr_un peer_address{};
            peer_address.sun_family = AF_UNIX;
            strcpy(peer_address.sun_path, m_peer_name.c_str());
            if (m_peer_name.at(0) == '#'){
                peer_address.sun_path[0] = 0;
            }
            auto address_length = sizeof(peer_address.sun_family) + m_peer_name.size();
            bytes_sent = ::sendto(m_socket, &byte, 1, MSG_NOSIGNAL, reinterpret_cast< struct sockaddr* >(&peer_address), address_length);
        }
    }
    if (static_cast< int8_t >(bytes_sent) < 0) {
        tristan::sockets::Error error{};
        switch (errno) {
            case EAGAIN: {
                error = tristan::sockets::Error::WRITE_TRY_AGAIN;
                break;
            }
#if defined(EWOULDBLOCK) && EWOULDBLOCK != EAGAIN
            case EWOULDBLOCK: {
                error = tristan::sockets::Error::WRITE_TRY_AGAIN;
                break;
            }
#endif
            case EBADF: {
                error = tristan::sockets::Error::WRITE_BAD_FILE_DESCRIPTOR;
                break;
            }
            case EDESTADDRREQ: {
                error = tristan::sockets::Error::WRITE_DESTINATION_ADDRESS;
                break;
            }
            case EFAULT: {
                error = tristan::sockets::Error::WRITE_BUFFER_OUT_OF_RANGE;
                break;
            }
            case EFBIG: {
                error = tristan::sockets::Error::WRITE_BIG;
                break;
            }
            case EINTR: {
                error = tristan::sockets::Error::WRITE_INTERRUPTED;
                break;
            }
            case EINVAL: {
                error = tristan::sockets::Error::WRITE_INVALID_ARGUMENT;
                break;
            }
            case EIO: {
                error = tristan::sockets::Error::WRITE_LOW_LEVEL_IO;
                break;
            }
            case ENOSPC: {
                error = tristan::sockets::Error::WRITE_NO_SPACE;
                break;
            }
            case EPERM: {
                error = tristan::sockets::Error::WRITE_NOT_PERMITTED;
                break;
            }
            case EPIPE: {
                error = tristan::sockets::Error::WRITE_PIPE;
                break;
            }
        }
        m_error = tristan::sockets::makeError(error);
    }
    return bytes_sent;
}

auto tristan::sockets::IpcSocket::write(const std::vector< uint8_t >& data, uint16_t size, uint64_t offset) -> uint64_t {
    if (m_socket == -1) {
        m_error = tristan::sockets::makeError(tristan::sockets::Error::SOCKET_NOT_INITIALISED);
        return 0;
    }
    if (data.empty()) {
        return 0;
    }

    uint64_t bytes_sent = 0;
    uint64_t l_size = (size == 0 ? data.size() : size);
    if (m_connected) {
        bytes_sent = ::write(m_socket, data.data() + offset, l_size);
    } else {
        if (m_type == tristan::sockets::SocketType::STREAM) {
            m_error = tristan::sockets::makeError(tristan::sockets::Error::SOCKET_NOT_CONNECTED);
        } else {
            sockaddr_un peer_address{};
            peer_address.sun_family = AF_UNIX;
            strcpy(peer_address.sun_path, m_peer_name.c_str());
            if (m_peer_name.at(0) == '#'){
                peer_address.sun_path[0] = 0;
            }
            auto address_length = sizeof(peer_address.sun_family) + m_peer_name.size();
            bytes_sent = ::sendto(m_socket, data.data() + offset, l_size, MSG_NOSIGNAL, reinterpret_cast< struct sockaddr* >(&peer_address), address_length);
        }
    }
    if (static_cast< int8_t >(bytes_sent) < 0) {
        tristan::sockets::Error error{};
        switch (errno) {
            case EAGAIN: {
                error = tristan::sockets::Error::WRITE_TRY_AGAIN;
                break;
            }
#if defined(EWOULDBLOCK) && EWOULDBLOCK != EAGAIN
            case EWOULDBLOCK: {
                error = tristan::sockets::Error::WRITE_TRY_AGAIN;
                break;
            }
#endif
            case EBADF: {
                error = tristan::sockets::Error::WRITE_BAD_FILE_DESCRIPTOR;
                break;
            }
            case EDESTADDRREQ: {
                error = tristan::sockets::Error::WRITE_DESTINATION_ADDRESS;
                break;
            }
            case EFAULT: {
                error = tristan::sockets::Error::WRITE_BUFFER_OUT_OF_RANGE;
                break;
            }
            case EFBIG: {
                error = tristan::sockets::Error::WRITE_BIG;
                break;
            }
            case EINTR: {
                error = tristan::sockets::Error::WRITE_INTERRUPTED;
                break;
            }
            case EINVAL: {
                error = tristan::sockets::Error::WRITE_INVALID_ARGUMENT;
                break;
            }
            case EIO: {
                error = tristan::sockets::Error::WRITE_LOW_LEVEL_IO;
                break;
            }
            case ENOSPC: {
                error = tristan::sockets::Error::WRITE_NO_SPACE;
                break;
            }
            case EPERM: {
                error = tristan::sockets::Error::WRITE_NOT_PERMITTED;
                break;
            }
            case EPIPE: {
                error = tristan::sockets::Error::WRITE_PIPE;
                break;
            }
        }
        m_error = tristan::sockets::makeError(error);
    }
    return bytes_sent;
}

auto tristan::sockets::IpcSocket::read() -> uint8_t {
    uint8_t byte = 0;

    auto status = ::read(m_socket, &byte, 1);
    if (status < 0) {
        tristan::sockets::Error error{};
        switch (errno) {
            case EAGAIN: {
                error = tristan::sockets::Error::READ_TRY_AGAIN;
                break;
            }
#if defined(EWOULDBLOCK) && EWOULDBLOCK != EAGAIN
            case EWOULDBLOCK: {
                error = tristan::sockets::Error::READ_TRY_AGAIN;
                break;
            }
#endif
            case EBADF: {
                error = tristan::sockets::Error::READ_BAD_FILE_DESCRIPTOR;
                break;
            }
            case EFAULT: {
                error = tristan::sockets::Error::READ_BUFFER_OUT_OF_RANGE;
                break;
            }
            case EINTR: {
                error = tristan::sockets::Error::READ_INTERRUPTED;
                break;
            }
            case EINVAL: {
                error = tristan::sockets::Error::READ_INVALID_FILE_DESCRIPTOR;
                break;
            }
            case EIO: {
                error = tristan::sockets::Error::READ_IO;
                break;
            }
            case EISDIR: {
                error = tristan::sockets::Error::READ_IS_DIRECTORY;
                break;
            }
        }
        m_error = tristan::sockets::makeError(error);
    }
    if (byte == 255) {
        m_error = tristan::sockets::makeError(tristan::sockets::Error::READ_EOF);
        byte = 0;
    }
    return byte;
}

auto tristan::sockets::IpcSocket::read(uint16_t size) -> std::vector< uint8_t > {
    if (size == 0) {
        return {};
    }

    std::vector< uint8_t > data;
    data.resize(size);
    auto status = ::read(m_socket, data.data(), size);
    if (status < 0) {
        tristan::sockets::Error error{};
        switch (errno) {
            case EAGAIN: {
                error = tristan::sockets::Error::READ_TRY_AGAIN;
                break;
            }
            case EBADF: {
                error = tristan::sockets::Error::READ_BAD_FILE_DESCRIPTOR;
                break;
            }
            case EFAULT: {
                error = tristan::sockets::Error::READ_BUFFER_OUT_OF_RANGE;
                break;
            }
            case EINTR: {
                error = tristan::sockets::Error::READ_INTERRUPTED;
                break;
            }
            case EINVAL: {
                error = tristan::sockets::Error::READ_INVALID_FILE_DESCRIPTOR;
                break;
            }
            case EIO: {
                error = tristan::sockets::Error::READ_IO;
                break;
            }
            case EISDIR: {
                error = tristan::sockets::Error::READ_IS_DIRECTORY;
                break;
            }
        }
        m_error = tristan::sockets::makeError(error);
    }

    //    for (uint16_t i = 0; i < size; ++i) {
    //        uint8_t byte = InetSocket::read();
    //        if (m_error) {
    //            break;
    //        }
    //        data.push_back(byte);
    //    }
    data.shrink_to_fit();
    if (data.at(0) == 0) {
        return {};
    }
    return data;
}

auto tristan::sockets::IpcSocket::readUntil(uint8_t delimiter) -> std::vector< uint8_t > {
    std::vector< uint8_t > data;

    while (true) {
        uint8_t byte = IpcSocket::read();
        if (m_error || byte == 0) {
            break;
        }
        if (byte == delimiter) {
            m_error = tristan::sockets::makeError(tristan::sockets::Error::READ_DONE);
            break;
        }
        data.push_back(byte);
    }
    if (not data.empty()) {
        data.shrink_to_fit();
    }
    return data;
}

auto tristan::sockets::IpcSocket::readUntil(const std::vector< uint8_t >& delimiter) -> std::vector< uint8_t > {
    std::vector< uint8_t > data;
    data.reserve(delimiter.size());
    while (true) {
        uint8_t byte = IpcSocket::read();
        if (m_error || byte == 0) {
            break;
        }
        data.push_back(byte);
        if (data.size() >= delimiter.size()) {
            std::vector< uint8_t > to_compare(data.end() - static_cast< int64_t >(delimiter.size()), data.end());
            if (to_compare == delimiter) {
                m_error = tristan::sockets::makeError(tristan::sockets::Error::READ_DONE);
                break;
            }
        } else if (data.size() == delimiter.size() && data == delimiter) {
            m_error = tristan::sockets::makeError(tristan::sockets::Error::READ_DONE);
            break;
        }
    }

    if (m_error.value() == static_cast< int >(tristan::sockets::Error::READ_DONE)) {
        data.erase(data.end() - static_cast< int64_t >(delimiter.size()), data.end());
    }
    if (not data.empty()) {
        data.shrink_to_fit();
    }
    return data;
}

auto tristan::sockets::IpcSocket::name() const noexcept -> const std::string& { return m_name; }

auto tristan::sockets::IpcSocket::peerName() const noexcept -> const std::string& { return m_peer_name; }

auto tristan::sockets::IpcSocket::error() const noexcept -> std::error_code { return m_error; }

auto tristan::sockets::IpcSocket::nonBlocking() const noexcept -> bool { return m_non_blocking; }

auto tristan::sockets::IpcSocket::connected() const noexcept -> bool { return m_connected; }

tristan::sockets::IpcSocket::IpcSocket(bool) :
    m_socket(-1),
    m_type(tristan::sockets::SocketType::STREAM),
    m_global_namespace(false),
    m_peer_global_namespace(false),
    m_non_blocking(false),
    m_bound(false),
    m_listening(false),
    m_connected(false) { }
