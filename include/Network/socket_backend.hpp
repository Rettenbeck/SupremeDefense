#pragma once

#include <Util/basic.hpp>
#include <EventDispatcher/include.hpp>


namespace SupDef {

    class SocketBackend : public Listener {
        public:
            virtual bool startAsClient(const std::string& ip, unsigned short port) = 0;
            virtual bool startAsServer(unsigned short port) = 0;
            virtual bool checkForClients() = 0;
            virtual void closeServer() = 0;
            virtual void send(const std::string& data) = 0;
            virtual std::optional<std::string> receive() = 0;
            virtual void update() = 0;
            virtual ~SocketBackend() = default;

            void blockGame(bool blocked = true) {
                assert(globalDispatcher);
                globalDispatcher->dispatch<GameBlockedByNetworkEvent>(blocked);
            }

    };

    using USocketBackend = std::unique_ptr<SocketBackend>;

}
