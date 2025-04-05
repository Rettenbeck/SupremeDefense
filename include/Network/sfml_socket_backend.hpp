#include <Network/socket_backend.hpp>

#pragma once


namespace SupDef {

    class SFMLSocketBackend : public SocketBackend {
        // manages sf::TcpSocket(s), select() logic, queues, etc.
    };

    using USFMLSocketBackend = std::unique_ptr<SFMLSocketBackend>;

}
