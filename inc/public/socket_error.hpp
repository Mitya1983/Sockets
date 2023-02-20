#ifndef SOCKET_ERROR_HPP
#define SOCKET_ERROR_HPP

#include <cstdint>
#include <system_error>

namespace tristan::sockets {

    enum class Error : uint8_t {
        SUCCESS,
        SOCKET_PROTOCOL_NOT_SUPPORTED,
        SOCKET_PROCESS_TABLE_IS_FULL,
        SOCKET_SYSTEM_TABLE_IS_FULL,
        SOCKET_NOT_ENOUGH_PERMISSIONS,
        SOCKET_NOT_ENOUGH_MEMORY,
        SOCKET_WRONG_PROTOCOL,
        SOCKET_WRONG_IP_FORMAT,
        SOCKET_NOT_INITIALISED,
        SOCKET_FCNTL_ERROR,
        SOCKET_NOT_CONNECTED,
        SOCKET_TIMED_OUT,
        BIND_NOT_ENOUGH_PERMISSIONS,
        BIND_ADDRESS_IN_USE,
        BIND_BAD_FILE_DESCRIPTOR,
        BIND_ALREADY_BOUND,
        BIND_FILE_DESCRIPTOR_IS_NOT_SOCKET,
        BIND_ADDRESS_NOT_AVAILABLE,
        BIND_ADDRESS_OUTSIDE_USER_SPACE,
        BIND_TO_MANY_SYMBOLIC_LINKS,
        BIND_NAME_TO_LONG,
        BIND_NO_ENTRY,
        BIND_NO_MEMORY,
        BIND_NOT_DIRECTORY,
        BIND_READ_ONLY_FS,
        LISTEN_ADDRESS_IN_USE,
        LISTEN_BAD_FILE_DESCRIPTOR,
        LISTEN_FILE_DESCRIPTOR_IS_NOT_SOCKET,
        LISTEN_PROTOCOL_NOT_SUPPORTED,
        LISTEN_ALREADY_CONNECTED,
        LISTEN_NOT_BOUND,
        ACCEPT_SOCKET_IS_NOT_IN_LISTEN_MODE,
        ACCEPT_ALREADY_CONNECTED,
        ACCEPT_NOT_BOUND,
        ACCEPT_TRY_AGAIN,
        ACCEPT_BAD_FILE_DESCRIPTOR,
        ACCEPT_CONNECTION_ABORTED,
        ACCEPT_ADDRESS_OUTSIDE_USER_SPACE,
        ACCEPT_INTERRUPTED,
        ACCEPT_INVALID_VALUE,
        ACCEPT_PER_PROCESS_LIMIT_REACHED,
        ACCEPT_SYSTEM_WIDE_LIMIT_REACHED,
        ACCEPT_NOT_ENOUPH_MEMORY,
        ACCEPT_FILE_DESCRIPTOR_IS_NOT_SOCKET,
        ACCEPT_OPTION_IS_NOT_SUPPORTED,
        ACCEPT_FIREWALL,
        ACCEPT_PROTOCOL_ERROR,
        CONNECT_SOCKET_IS_IN_LISTEN_MODE,
        CONNECT_NOT_ENOUGH_PERMISSIONS,
        CONNECT_ADDRESS_IN_USE,
        CONNECT_ADDRESS_NOT_AVAILABLE,
        CONNECT_AF_NOT_SUPPORTED,
        CONNECT_TRY_AGAIN,
        CONNECT_ALREADY_IN_PROCESS,
        CONNECT_BAD_FILE_DESCRIPTOR,
        CONNECT_CONNECTION_REFUSED,
        CONNECT_ADDRESS_OUTSIDE_USER_SPACE,
        CONNECT_IN_PROGRESS,
        CONNECT_INTERRUPTED,
        CONNECT_CONNECTED,
        CONNECT_NETWORK_UNREACHABLE,
        CONNECT_FILE_DESCRIPTOR_IS_NOT_SOCKET,
        CONNECT_PROTOCOL_NOT_SUPPORTED,
        SSL_METHOD_ERROR,
        SSL_CONTEXT_ERROR,
        SSL_INIT_ERROR,
        SSL_TRY_AGAIN,
        SSL_CONNECT_ERROR,
        SSL_CERTIFICATE_ERROR,
        SSL_CERTIFICATE_VERIFICATION_HOST,
        SSL_CERTIFICATE_VERIFICATION_START_DATE,
        SSL_CERTIFICATE_VERIFICATION_END_DATE,
        SSL_CERTIFICATE_VALIDATION_FAILED,
        SSL_CLOSED_BY_PEER,
        SSL_IO_ERROR,
        SSL_FATAL_ERROR,
        SSL_UNKNOWN_ERROR,
        WRITE_ACCESS,
        WRITE_TRY_AGAIN,
        WRITE_ALREADY,
        WRITE_BAD_FILE_DESCRIPTOR,
        WRITE_CONNECTION_RESET,
        WRITE_DESTINATION_ADDRESS,
        WRITE_USER_QUOTA,
        WRITE_BUFFER_OUT_OF_RANGE,
        WRITE_INTERRUPTED,
        WRITE_INVALID_ARGUMENT,
        WRITE_IS_CONNECTED,
        WRITE_MESSAGE_SIZE,
        WRITE_NO_BUFFER,
        WRITE_NO_MEMORY,
        WRITE_NOT_CONNECTED,
        WRITE_NOT_SOCKET,
        WRITE_NOT_SUPPORTED,
        WRITE_PIPE,
        READ_TRY_AGAIN,
        READ_BAD_FILE_DESCRIPTOR,
        READ_CONNECTION_REFUSED,
        READ_BUFFER_OUT_OF_RANGE,
        READ_INTERRUPTED,
        READ_INVALID_FILE_DESCRIPTOR,
        READ_NO_MEMORY,
        READ_NOT_CONNECTED,
        READ_NOT_SOCKET,
        READ_EOF,
        READ_CONNECTION_RESET,
        READ_DONE,
        SHUTDOWN_INVALID_SOCKET_ARGUMENT,
        SHUTDOWN_INVALID_SHUTDOWN_OPTION_PROVIDED,
        SHUTDOWN_NOT_CONNECTED,
        SHUTDOWN_INVALID_FILE_DESCRIPTOR,
        SHUTDOWN_NOT_ENOUPH_MEMORY
    };

    /**
     * \brief Creates std::error_code object that stores error information.
     * \param error_code SocketErrors
     * \return std::error_code
     */
    [[nodiscard]] auto makeError(Error error_code) -> std::error_code;

}  // namespace tristan::sockets

namespace std {
    /**
     * \brief //This is needed to specialise the standard type trait.
     */
    template <> struct is_error_code_enum< tristan::sockets::Error > : true_type { };
}  // namespace std

#endif  //SOCKET_ERROR_HPP
