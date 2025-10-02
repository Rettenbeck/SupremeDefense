#pragma once

#include <Network/constants.hpp>
#include <Network/socket_backend.hpp>
#include <Network/network_router.hpp>
#include <SFML/Network.hpp>


namespace SupDef {

    struct NetClient {
        ClientId id = CLIENT_ID_SERVER;
        std::unique_ptr<sf::TcpSocket> socket = nullptr;
        bool ok = true;
    };

    using UNetClient = std::unique_ptr<NetClient>;
    using NetClients = std::vector<UNetClient>;

    class SFMLSocketBackend : public SocketBackend {
        public:
            using Clock = std::chrono::high_resolution_clock;
            using Duration = std::chrono::duration<double, std::milli>;

            SFMLSocketBackend() {
                networkRouter = std::make_unique<NetworkRouter>();
            }

            void initialize() override {
                SUBSCRIBE(RequestServerListRefreshEvent)
            }

            bool start(unsigned short discoveryPort_, unsigned short connectionPort_) override {
                discoveryPort = discoveryPort_;
                connectionPort = connectionPort_;

                listener = std::make_unique<sf::TcpListener>();
                listener->setBlocking(false);

                udp = std::make_unique<sf::UdpSocket>();
                if (udp->bind(discoveryPort) != sf::Socket::Status::Done) return false;

                udp->setBlocking(false);

                server.reset();
                clients.clear();

                std::stringstream ss;
                // ss << "Server (" << Clock::now() << ")";
                ss << "Server Test";
                serverName = ss.str();

                // waitingForDiscovery = false;
                myClientId = CLIENT_ID_SERVER;
                acceptClients = false;
                role = Role::None;

                return true;
            }

            Role getRole() noexcept { return role; }

            NetResult connectToServer(std::string serverName) override {
                for (auto& s : servers) {
                    if (s.server_name == serverName) {
                        return connectToServer(Endpoint(s.ip, s.port));
                    }
                }
                return NetResult::Error("Server not found by name");
            }

            NetResult connectToServer(const Endpoint& addr) override {
                if (role != Role::None) {
                    return NetResult::Error("Cannot connect to server due to wrong role");
                }

                auto ip = sf::IpAddress::resolve(addr.ip);
                if (!ip.has_value()) {
                    return NetResult::Error("Failed to resolve host: " + addr.ip);
                }

                server = std::make_unique<sf::TcpSocket>();
                server->setBlocking(true);

                auto status = server->connect(ip.value(), addr.port, sf::seconds(NETWORK_MAX_WAIT));
                server->setBlocking(false);

                if (status != sf::Socket::Status::Done) {
                    server.reset();
                    return NetResult::Error("Connecting to server failed");
                }

                myClientId = CLIENT_ID_SERVER;
                role = Role::Client;
                return NetResult::Ok();
            }

            NetResult kickClient(ClientId id, std::string reason = "") override {
                if (role != Role::Server) {
                    return NetResult::Error("Cannot kick client due to wrong role");
                }
                auto index = findClient(id);
                if (index < 0) {
                    return NetResult::Error("Client id not found");
                }

                assert(index > 0 && index < clients.size());
                auto& client = clients[index];

                auto j = generateInitialMessage(NETWORK_TYPE_CLIENT_KICK);
                j[NETWORK_REASON] = reason;
                auto result = sendToSocket(client->socket.get(), j.dump());

                client->ok = false;
                client->socket->disconnect();
                return NetResult::Ok();
            }

            NetResult removeClient(ClientId id) override {
                if (role != Role::Server) {
                    return NetResult::Error("Cannot kick client due to wrong role");
                }
                auto index = findClient(id);
                if (index < 0) {
                    return NetResult::Error("Client id not found");
                }
                assert(index > 0 && index < clients.size());
                auto& client = clients[index];
                client->ok = false;
                client->socket->disconnect();
                return NetResult::Ok();
            }

            void disconnectFromServer() noexcept override {
                if (role != Role::Client) return;
                if (server) {
                    send(generateInitialMessage(NETWORK_TYPE_DISCONNECT_FROM_CLIENT).dump());
                    server->disconnect();
                }
                server.reset();
                role == Role::None;
            }

            bool isConnected() const noexcept override {
                if (role != Role::Client) return false;
                if (server.get() == nullptr) return false;
                return server->getRemoteAddress().has_value();
            }

            NetResult openServer(unsigned short port) override {
                if (role != Role::None) {
                    return NetResult::Error("Cannot open server due to wrong role");
                }
                assert(listener);
                if (listener->listen(connectionPort) != sf::Socket::Status::Done) {
                    return NetResult::Error("Cannot open server due to listener");
                }
                clients.clear();
                acceptClients = true;
                role = Role::Server;
                return NetResult::Ok();
            }

            void closeServer() noexcept override {
                if (role != Role::Server) return;
                assert(listener);
                listener->close();
                for (auto& c : clients) kickClient(c->id, NETWORK_REASON_SERVER_CLOSE);
                clients.clear();
                role = Role::None;
            }

            void checkForNewClients() {
                assert(listener);
                auto socket = std::make_unique<sf::TcpSocket>();
                if (listener->accept(*socket) == sf::Socket::Status::Done) {
                    socket->setBlocking(false);
                    if (acceptClients) {
                        auto client = std::make_unique<NetClient>();
                        auto newId = id_counter++;
                        client->id = newId;
                        client->socket = (std::move(socket));
                        client->ok = true;
                        clients.push_back(std::move(client));
                        onNewClient(newId);
                    } else {
                        auto j = generateInitialMessage(NETWORK_TYPE_JOIN_REJECTED);
                        j[NETWORK_REASON] = rejectingClientMessage;
                        sendToSocket(socket.get(), j.dump());
                    }
                }
            }

            ClientIds getClients() const override {
                ClientIds result;
                for (auto& c : clients) result.push_back(c->id);
                return result;
            }

            NetResult send(std::string message) override {
                NetResult result;
                switch(role) {
                    case Role::None:
                        return NetResult::Error("Cannot send message due to no role");
                    case Role::Server:
                        for(auto& c : clients) {
                            result = sendToSocket(c->socket.get(), message);
                            if (!result.ok) return result;
                        }
                        break;
                    case Role::Client:
                        result = sendToSocket(server.get(), message);
                        if (!result.ok) return result;
                        break;
                    default:
                        assert(false);
                }
                return NetResult::Ok();
            }

            NetResult sendToClient(ClientId id, std::string message) override {
                auto index = findClient(id);
                if (index < 0) {
                    return NetResult::Error("Client id not found");
                }
                assert(index > 0 && index < clients.size());
                auto& client = clients[index];
                return sendToSocket(client->socket.get(), message);
            }

            NetResult sendToSocket(sf::TcpSocket* socket, std::string message) {
                assert(socket);
                sf::Packet packet;
                packet >> message;
                if (socket->send(packet) != sf::Socket::Status::Done) {
                    return NetResult::Error("Tcp message could not be sent");
                }
                return NetResult::Ok();
            }

            NetResult requestDiscovery() override {
                if (role != Role::None) {
                    return NetResult::Error("Cannot ask for discovery due to wrong role");
                }

                assert(udp);
                sf::Packet packet;
                packet << buildDiscoveryRequest();

                auto status = udp->send(packet, sf::IpAddress::Broadcast, discoveryPort);
                if (status != sf::Socket::Status::Done) {
                    return NetResult::Error("Failed to send discovery request");
                }

                servers.clear();
                return NetResult::Ok();
            }

            NetResult replyDiscovery(sf::IpAddress sender, unsigned short port) {
                assert(udp);
                sf::Packet packet;
                packet << buildDiscoveryReply();
                auto status = udp->send(packet, sender, port);

                if (status == sf::Socket::Status::NotReady) {
                    return NetResult::Error("UDP send not ready");
                }
                if (status != sf::Socket::Status::Done) {
                    return NetResult::Error("Failed to send discovery reply");
                }

                return NetResult::Ok();
            }

            NetResult checkDiscoveryRequest() {
                if (role != Role::Server) {
                    return NetResult::Error("Cannot check for discovery request due to wrong role");
                }

                constexpr int kMaxPerTick = 16;
                int processed = 0;

                while (processed < kMaxPerTick) {
                    sf::Packet packet;
                    auto sender = sf::IpAddress::Any;
                    unsigned short senderPort = 0;
                    std::optional<sf::IpAddress> opt_sender = sender;

                    assert(udp);
                    auto status = udp->receive(packet, opt_sender, senderPort);

                    if (status == sf::Socket::Status::NotReady) {
                        // No more packets right now — this is fine in non-blocking mode.
                        break;
                    }
                    if (status != sf::Socket::Status::Done) {
                        // Real error
                        return NetResult::Error("Failed to receive discovery request");
                    }

                    // Deserialize and validate
                    if (!analyzeDiscoveryRequest(packet)) {
                        ++processed;
                        continue;
                    }

                    auto replyRes = replyDiscovery(sender, senderPort);
                    if (!replyRes.ok) {
                        // You might also choose to log and continue instead of bailing.
                        return replyRes;
                    }

                    ++processed;
                }

                return NetResult::Ok();
            }

            NetResult checkDiscoveryReply() {
                if (role != Role::None) {
                    return NetResult::Error("Cannot check for discovery reply due to wrong role");
                }

                constexpr int kMaxPerTick = 16;
                int processed = 0;

                while (processed < kMaxPerTick) {
                    sf::Packet packet;
                    auto sender = sf::IpAddress::Any;
                    unsigned short senderPort = 0;
                    std::optional<sf::IpAddress> opt_sender = sender;

                    assert(udp);
                    auto status = udp->receive(packet, opt_sender, senderPort);

                    if (status == sf::Socket::Status::NotReady) {
                        // No more packets right now — this is fine in non-blocking mode.
                        break;
                    }
                    if (status != sf::Socket::Status::Done) {
                        // Real error
                        return NetResult::Error("Failed to receive discovery reply");
                    }

                    // Deserialize and validate
                    auto reply = analyzeDiscoveryReply(packet);
                    if (!reply.ok) {
                        ++processed;
                        continue;
                    }

                    reply.ip = sender.toString();
                    servers.push_back(reply);
                    ++processed;
                }

                return NetResult::Ok();
            }

            json generateInitialMessage(const int message_type) {
                json j;
                j[NETWORK_MESSAGE_TYPE] = message_type;
                j[NETWORK_VERSION] = NETWORK_VERSION_CURRENT;
                return j;
            }

            NetMessage analyzeMessage(sf::Packet& packet) {
                std::string data;
                if (!(packet >> data)) {
                    NetMessage result;
                    result.error = "Could not retrieve data from packet";
                    return result;
                }
                return analyzeMessage(data);
            }

            NetMessage analyzeMessage(std::string msg) {
                NetMessage result;
                try {
                    result.j = nlohmann::json::parse(msg);
                } catch (nlohmann::json::parse_error& e) {
                    std::stringstream ss;
                    ss << "Parse error: " << e.what() << '\n';
                    result.error = ss.str();
                    return result;
                }
                
                if (!result.j.contains(NETWORK_MESSAGE_TYPE)) {
                    result.error = "Message does not contain message type";
                    return result;
                }
                if (!result.j.contains(NETWORK_VERSION)) {
                    result.error = "Message does not contain version info";
                    return result;
                }
                if (result.j[NETWORK_VERSION] != NETWORK_VERSION_CURRENT) {
                    result.error = "Message does not contain incorrect version info";
                    return result;
                }

                result.type = result.j[NETWORK_MESSAGE_TYPE];
                result.ok = true;
                return result;
            }

            std::string buildDiscoveryRequest() {
                return generateInitialMessage(NETWORK_TYPE_DISCOVERY_REQUEST).dump();
            }

            std::string buildDiscoveryReply() {
                auto j = generateInitialMessage(NETWORK_TYPE_DISCOVERY_REPLY);
                j[NETWORK_CONNECTION_PORT] = connectionPort;
                j[NETWORK_SERVERNAME] = getServerName();
                return j.dump();
            }

            bool analyzeDiscoveryRequest(sf::Packet& packet) {
                auto result = analyzeMessage(packet);
                if (!result.ok) return false;
                if (result.type != NETWORK_TYPE_DISCOVERY_REQUEST) return false;
                return true;
            }

            DiscoveryReply analyzeDiscoveryReply(sf::Packet& packet) {
                DiscoveryReply reply;
                auto result = analyzeMessage(packet);

                if (!result.ok) return reply;
                if (result.type != NETWORK_TYPE_DISCOVERY_REPLY) return reply;

                if (!result.j.contains(NETWORK_CONNECTION_PORT)) return reply;
                if (!result.j.contains(NETWORK_SERVERNAME)) return reply;
                
                reply.port = result.j[NETWORK_CONNECTION_PORT];
                reply.server_name = result.j[NETWORK_SERVERNAME];
                reply.ok = true;

                return reply;
            }

            void setToAcceptClients() override {
                acceptClients = true;
                rejectingClientMessage = "";
            }

            void setToRejectClients(std::string message) override {
                acceptClients = false;
                rejectingClientMessage = message;
            }

            DiscoveryReplies getServerList() override {
                return servers;
            }

            std::pair<bool, NetMessage> receiveFromSocket(sf::TcpSocket* socket) {
                assert(socket);
                sf::Packet packet;

                auto status = socket->receive(packet);
                if (status == sf::Socket::Status::Done) {
                    auto received = analyzeMessage(packet);
                    if (received.ok) return {true, received};
                } else if (status == sf::Socket::Status::Disconnected) {
                    socket->disconnect();
                    return {false, NetMessage::NoMessage()};
                }
                return {true, NetMessage::NoMessage()};
            }

            NetMessages receive() {
                NetMessages result;
                NetMessage received;
                bool connected;

                switch (role) {
                    case Role::None:
                        return result;
                    case Role::Server:
                        for (auto& c : clients) {
                            std::tie(connected, received) = receiveFromSocket(c->socket.get());
                            if (!c->ok) continue;
                            if (received.ok) result.push_back(received);
                            if (!connected) {
                                removeClient(received.clientId);
                                onClientDisconnected(received.clientId);
                            }
                        }
                        break;
                    case Role::Client:
                        std::tie(connected, received) = receiveFromSocket(server.get());
                        if (received.ok) result.push_back(received);
                        if (!connected) {
                            disconnectFromServer();
                            onServerDisconnected();
                        }
                        break;
                    default:
                        assert(false);
                }

                return result;
            }

            void processInternally(NetMessage msg) {
                std::string reason = "";
                switch (msg.type) {
                    case NETWORK_TYPE_JOIN_ACCEPTED:
                        if (msg.j.contains(NETWORK_ASSIGNED_CLIENT_ID)) {
                            myClientId = msg.j[NETWORK_ASSIGNED_CLIENT_ID];
                        }
                        break;
                    case NETWORK_TYPE_JOIN_REJECTED:
                        if (msg.j.contains(NETWORK_REASON)) {
                            reason = msg.j[NETWORK_REASON];
                        }
                        LOG_INFO("Rejected for reason: &1", reason)
                        break;
                    case NETWORK_TYPE_CLIENT_KICK:
                        if (msg.j.contains(NETWORK_REASON)) {
                            reason = msg.j[NETWORK_REASON];
                        }
                        LOG_INFO("Kicked for reason: &1", reason)
                        disconnectFromServer();
                        onServerDisconnect();
                        break;
                    case NETWORK_TYPE_DISCONNECT_FROM_CLIENT:
                        if (msg.j.contains(NETWORK_ASSIGNED_CLIENT_ID)) {
                            ClientId id = msg.j[NETWORK_ASSIGNED_CLIENT_ID];
                            LOG_INFO("Player &1 disconnected", id)
                            removeClient(id);
                            onClientDisconnect(id);
                        }
                        break;
                }
            }

            void clearDisconnectedSockets() {
                clients.erase(
                    std::remove_if(clients.begin(), clients.end(),
                                    [](const std::unique_ptr<NetClient>& client) { return !client->ok; } ),
                                    clients.end() );
            }

            std::string getServerName() override { return serverName; }
            void setServerName(std::string name) override { serverName = name; }

            void update() noexcept override {
                NetResult result;
                NetMessages received;

                switch (role) {
                    case Role::None:
                        result = checkDiscoveryReply();
                        if (!result.ok) LOG_ERROR(result.error);
                        break;
                    case Role::Server:
                        result = checkDiscoveryRequest();
                        if (!result.ok) LOG_ERROR(result.error);
                        received = receive();
                        break;
                    case Role::Client:
                        received = receive();
                        break;
                    default:
                        assert(false);
                }

                assert(networkRouter);
                for (auto& msg : received) {
                    processInternally(msg);
                    networkRouter->process(role, msg);
                }

                clearDisconnectedSockets();
            }

            int findClient(ClientId id) {
                for (int i = 0; i < clients.size(); i++) {
                    if (clients[i]->id == id) return i;
                }
                return -1;
            }

            void onNewClient(ClientId id) {
                auto j = generateInitialMessage(NETWORK_TYPE_JOIN_ACCEPTED);
                j[NETWORK_ASSIGNED_CLIENT_ID] = id;
                sendToClient(id, j.dump());
            }

            void onClientDisconnected(ClientId id) { // Disconnected unexpectedly
                removeClient(id);
            }

            void onClientDisconnect(ClientId id) { // Disconnected by message
                removeClient(id);
            }

            void onServerDisconnected() { // Disconnected unexpectedly
                //
            }

            void onServerDisconnect() { // Disconnected by message
                //
            }

            DEFINE_EVENT_CALLBACK_BEGIN(RequestServerListRefreshEvent) {
                auto result = requestDiscovery();
                dispatch<RequestServerListRefreshAnswerEvent>(result.ok, result.error);
            }

        private:
            Role role = Role::None;

            bool acceptClients = false;
            std::string rejectingClientMessage = "";

            ClientId id_counter = 0;
            ClientId myClientId = CLIENT_ID_SERVER;

            std::unique_ptr<sf::SocketSelector> selector;
            std::unique_ptr<sf::TcpListener> listener;
            std::unique_ptr<sf::UdpSocket> udp;
            std::unique_ptr<sf::TcpSocket> server;
            NetClients clients;

            UNetworkRouter networkRouter;

            unsigned short discoveryPort, connectionPort;

            std::string serverName = "";

            DiscoveryReplies servers;
            
            // bool waitingForDiscovery = false;
            // Clock::time_point startTime;

    };

    using USFMLSocketBackend = std::unique_ptr<SFMLSocketBackend>;

}
