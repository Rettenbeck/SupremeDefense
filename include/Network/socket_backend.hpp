#pragma once

#include <Network/network_structs.hpp>
#include <EventDispatcher/include.hpp>


namespace SupDef {

    class SocketBackend : public Listener {
        public:
            virtual ~SocketBackend() = default;

            virtual bool start(unsigned short discoveryPort_, unsigned short connectionPort_) = 0;

            virtual Role getRole() noexcept = 0;

            virtual NetResult connectToServer(std::string serverName) = 0;
            virtual NetResult connectToServer(const Endpoint& addr) = 0;   // Role::None -> Role::Client
            virtual NetResult kickClient(ClientId id, std::string reason = "") = 0;
            virtual NetResult removeClient(ClientId id) = 0;
            virtual void      disconnectFromServer() noexcept = 0;         // Role::Client -> Role::None
            virtual bool      isConnected() const noexcept = 0;            // Client: connected to server

            virtual NetResult openServer(unsigned short port) = 0;         // Role::None -> Role::Server
            virtual void      closeServer() noexcept = 0;                  // Role::Server -> Role::None
            virtual ClientIds getClients() const = 0;                      // server: current clients

            virtual NetResult send(std::string message) = 0;                        // Depending on role either sends to server or all clients
            virtual NetResult sendToClient(ClientId id, std::string message) = 0;   // Sends to a single client

            virtual NetResult requestDiscovery() = 0;
            virtual DiscoveryReplies getServerList() = 0;

            virtual void setToAcceptClients() = 0;
            virtual void setToRejectClients(std::string message) = 0;

            virtual std::string getServerName() = 0;
            virtual void setServerName(std::string name) = 0;

            virtual void update() noexcept = 0;

    };

    using USocketBackend = std::unique_ptr<SocketBackend>;

}
