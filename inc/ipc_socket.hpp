#ifndef IPC_SOCKET_HPP
#define IPC_SOCKET_HPP

#include "socket_common.hpp"

namespace tristan::sockets {

    class IpcSocket {
    public:
        explicit IpcSocket(SocketType socket_type = SocketType::STREAM);
        IpcSocket(const IpcSocket& other) = delete;
        IpcSocket(IpcSocket&& other) = delete;

        IpcSocket& operator=(const IpcSocket& other) = delete;
        IpcSocket& operator=(IpcSocket&& other) = delete;

        ~IpcSocket();

        void setName(const std::string& name, bool global_namespace);
        void setPeerName(const std::string& name, bool global_namespace);
        void setNonBlocking(bool non_blocking = true);
        void resetError();
        void bind();
        void listen(uint32_t connection_count_limit);
        void connect();
        void close();
        void shutdown();
        [[nodiscard]] auto accept() -> std::optional< std::unique_ptr< IpcSocket > >;

        auto write(uint8_t byte) -> uint8_t;

        auto write(const std::vector< uint8_t >& data, uint16_t size = 0, uint64_t offset = 0) -> uint64_t;

        template < class ObjectClassToSend >
        auto write(ObjectClassToSend object) -> uint64_t
            requires std::is_standard_layout_v< ObjectClassToSend >
        {
            std::vector< uint8_t > temp_data(reinterpret_cast< uint8_t* >(&object), reinterpret_cast< uint8_t* >(&object) + sizeof(object));
            return IpcSocket::write(temp_data);
        }

        [[nodiscard]] auto read() -> uint8_t;
        [[nodiscard]] auto read(uint16_t size) -> std::vector< uint8_t >;
        [[nodiscard]] auto readUntil(uint8_t delimiter) -> std::vector< uint8_t >;
        [[nodiscard]] auto readUntil(const std::vector< uint8_t >& delimiter) -> std::vector< uint8_t >;

        [[nodiscard]] auto name() const noexcept -> const std::string&;
        [[nodiscard]] auto peerName() const noexcept -> const std::string&;
        [[nodiscard]] auto error() const noexcept -> std::error_code;
        [[nodiscard]] auto nonBlocking() const noexcept -> bool;
        [[nodiscard]] auto connected() const noexcept -> bool;

    protected:
    private:
        explicit IpcSocket(bool);

        std::string m_name;
        std::string m_peer_name;

        std::error_code m_error;

        int32_t m_socket;

        SocketType m_type;

        bool m_global_namespace;
        bool m_peer_global_namespace;

        bool m_non_blocking;
        bool m_bound;
        bool m_listening;
        bool m_connected;
    };
}  // namespace tristan::sockets
#endif  //IPC_SOCKET_HPP
