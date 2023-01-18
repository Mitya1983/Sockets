#ifndef INET_SOCKET_HPP
#define INET_SOCKET_HPP

#include "socket_common.hpp"
#include "ssl.hpp"

namespace tristan::sockets {

    class InetSocket {
    public:
        explicit InetSocket(SocketType socket_type = SocketType::STREAM);
        InetSocket(const InetSocket& other) = delete;
        InetSocket(InetSocket&& other) = delete;

        InetSocket& operator=(const InetSocket& other) = delete;
        InetSocket& operator=(InetSocket&& other) = delete;

        ~InetSocket();

        void setHost(uint32_t ip, const std::string& host_name = "");
        void setPort(uint16_t port);
        void setNonBlocking(bool non_blocking = true);
        void resetError();
        void bind();
        void listen(uint32_t connection_count_limit);
        void connect(bool ssl = true);
        void close();
        void shutdown();
        [[nodiscard]] auto accept() -> std::optional<std::unique_ptr<InetSocket>>;

        auto write(uint8_t byte) -> uint8_t;

        auto write(const std::vector< uint8_t >& data, uint16_t size = 0, uint64_t offset = 0) -> uint64_t;

        template < class ObjectClassToSend >
        auto write(ObjectClassToSend object) -> uint64_t
            requires std::is_standard_layout_v< ObjectClassToSend >
        {
            std::vector< uint8_t > temp_data(reinterpret_cast< uint8_t* >(&object), reinterpret_cast< uint8_t* >(&object) + sizeof(object));
            return InetSocket::write(temp_data);
        }

        [[nodiscard]] auto read() -> uint8_t;
        [[nodiscard]] auto read(uint16_t size) -> std::vector< uint8_t >;
        [[nodiscard]] auto readUntil(uint8_t delimiter) -> std::vector< uint8_t >;
        [[nodiscard]] auto readUntil(const std::vector< uint8_t >& delimiter) -> std::vector< uint8_t >;

        [[nodiscard]] auto ip() const noexcept -> uint32_t;
        [[nodiscard]] auto port() const noexcept -> uint16_t;
        [[nodiscard]] auto error() const noexcept -> std::error_code;
        [[nodiscard]] auto nonBlocking() const noexcept -> bool;
        [[nodiscard]] auto connected() const noexcept -> bool;

    protected:
    private:

        explicit InetSocket(bool);

        std::string m_host_name;

        int32_t m_socket;
        uint32_t m_ip;

        std::error_code m_error;

        uint16_t m_port;

        std::unique_ptr<Ssl> m_ssl;
        SocketType m_type;


        bool m_non_blocking;
        bool m_bound;
        bool m_listening;
        bool m_not_ssl_connected;
        bool m_connected;
    };

}  // namespace tristan::sockets

#endif  //INET_SOCKET_HPP
