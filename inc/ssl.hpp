#ifndef OPEN_SSL_HPP
#define OPEN_SSL_HPP

#include <openssl/crypto.h>
#include <openssl/x509.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#include <string>
#include <vector>
#include <memory>

namespace tristan::sockets {

    class Ssl {
        friend class InetSocket;

    public:
        Ssl(const Ssl& other) = delete;
        Ssl(Ssl&& other) = delete;

        Ssl& operator=(const Ssl& other) = delete;
        Ssl& operator=(Ssl&& other) = delete;

        static auto create(int32_t socket) -> std::unique_ptr< Ssl >;

        ~Ssl();

    private:
        explicit Ssl(int32_t socket);

        auto connect() -> std::error_code;

        auto verifyHost(const std::string& host) -> bool;
        auto verifyIp(uint32_t ip) -> bool;
        auto verifyIp(uint64_t ip) -> bool;
        auto verifyStartDate() -> bool;
        auto verifyEndDate() -> bool;

        [[nodiscard]] auto write(uint8_t byte) -> std::pair< std::error_code, uint8_t >;

        [[nodiscard]] auto write(const std::vector< uint8_t >& data, uint16_t size = 0, uint64_t offset = 0) -> std::pair< std::error_code, uint64_t >;

        template < class ObjectClassToSend >
        auto write(ObjectClassToSend object) -> std::pair< std::error_code, uint64_t >
            requires std::is_standard_layout_v< ObjectClassToSend >
        {
            std::vector< uint8_t > temp_data(reinterpret_cast< uint8_t* >(object), reinterpret_cast< uint8_t* >(object) + sizeof(object));
            return Ssl::write(temp_data);
        }

        [[nodiscard]] auto read() -> std::pair< std::error_code, uint8_t >;
        [[nodiscard]] auto read(std::vector<uint8_t>& data, uint16_t size) -> std::pair< std::error_code, std::vector< uint8_t > >;

        void shutdown();

        SSL_CTX* m_context;
        SSL* m_ssl;
        X509* m_server_certificate;

        const SSL_METHOD* method;
    };

}  // namespace tristan::sockets

#endif  //OPEN_SSL_HPP
