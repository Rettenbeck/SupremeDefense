#include <Util/basic.hpp>
#include <EventDispatcher/include.hpp>

#pragma once


namespace SupDef {

    class SocketBackend : public Listener {
        public:
            virtual void send(const std::string& data, int playerID = -1) = 0;
            virtual std::optional<std::string> receive(int playerID = -1) = 0;
            virtual void update() = 0;
            virtual ~SocketBackend() = default;

            void blockGame(bool blocked = true) {
                assert(globalDispatcher);
                globalDispatcher->dispatch<GameBlockedEvent>(blocked);
            }

    };

    using USocketBackend = std::unique_ptr<SocketBackend>;

}
