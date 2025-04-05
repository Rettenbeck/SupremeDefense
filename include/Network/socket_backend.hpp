#include <Util/basic.hpp>

#pragma once


namespace SupDef {

    class SocketBackend {
        public:
            virtual void send(const std::string& data, int playerID = -1) = 0;
            virtual std::optional<std::string> receive(int playerID = -1) = 0;
            virtual void update() = 0;
            virtual ~SocketBackend() = default;

    };

    using USocketBackend = std::unique_ptr<SocketBackend>;

}
