#include <Network/socket_backend.hpp>

#pragma once


namespace SupDef {

    class SFMLSocketBackend : public SocketBackend {
        public:

            SFMLSocketBackend() {}
            
    };

    using USFMLSocketBackend = std::unique_ptr<SFMLSocketBackend>;

}
