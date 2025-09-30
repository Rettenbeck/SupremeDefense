#pragma once

#include <Util/util.hpp>
#include <App/constants.hpp>
#include <Action/action_queue.hpp>
#include <App/Log/logger.hpp>
#include <Network/network_structs.hpp>


namespace SupDef {

    class NetworkRouter {
        public:

            void process(Role role, NetMessage netMessage) {
                //
            }

    };

    using UNetworkRouter = std::unique_ptr<NetworkRouter>;

}
