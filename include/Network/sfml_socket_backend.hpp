#include <Network/socket_backend.hpp>
#include <SFML/Network.hpp>

#pragma once


namespace SupDef {

    class SFMLSocketBackend : public SocketBackend {
        private:
            sf::TcpSocket socket;
            sf::TcpListener listener;
            bool isServer = false;
            bool isConnected = false;
            std::string leftoverData;
        
        public:
            SFMLSocketBackend() {
                socket.setBlocking(false);
            }
        
            bool startAsClient(const std::string& ip, unsigned short port) {
                auto opt_ip = sf::IpAddress::resolve(ip);
                if (!opt_ip.has_value()) return false;

                if (socket.connect(opt_ip.value(), port) == sf::Socket::Status::Done) {
                    isServer = false;
                    isConnected = true;
                    socket.setBlocking(false);
                    return true;
                }
                return false;
            }
        
            bool startAsServer(unsigned short port) {
                if (listener.listen(port) != sf::Socket::Status::Done) return false;
                listener.setBlocking(false);
                sf::TcpSocket newClient;
                if (listener.accept(socket) == sf::Socket::Status::Done) {
                    isServer = true;
                    isConnected = true;
                    socket.setBlocking(false);
                    return true;
                }
                return false;
            }
        
            void send(const std::string& data) override {
                if (!isConnected) return;
        
                // Send length prefix first (4 bytes), then string
                uint32_t len = static_cast<uint32_t>(data.size());
                // len = htonl(len);  // ensure network byte order
        
                if (socket.send(&len, sizeof(len)) != sf::Socket::Status::Done) return;
                if (socket.send(data.c_str(), data.size()) != sf::Socket::Status::Done) return;
            }
        
            std::optional<std::string> receive() override {
                if (!isConnected) return std::nullopt;
        
                // Read message length (if not already buffered)
                uint32_t lenNet;
                std::size_t received;
        
                if (leftoverData.size() < sizeof(lenNet)) {
                    char buffer[4];
                    if (socket.receive(buffer, 4, received) != sf::Socket::Status::Done) return std::nullopt;
                    if (received < 4) return std::nullopt;
        
                    std::memcpy(&lenNet, buffer, 4);
                } else {
                    std::memcpy(&lenNet, leftoverData.data(), 4);
                    leftoverData.erase(0, 4);
                }
        
                // uint32_t len = ntohl(lenNet);
                uint32_t len = lenNet;
                std::string msg(len, '\0');
        
                if (socket.receive(&msg[0], len, received) != sf::Socket::Status::Done) return std::nullopt;
                if (received < len) return std::nullopt;
        
                return msg;
            }
        
            void update() override {
                // Not used in this minimal version
            }

    };

    using USFMLSocketBackend = std::unique_ptr<SFMLSocketBackend>;

}
