#include <Util/util.hpp>
#include <App/constants.hpp>
#include <Action/action_queue.hpp>
#include <Network/socket_backend.hpp>
#include <App/Log/logger.hpp>

#pragma once


namespace SupDef {

    struct NetworkPackage {
        bool forServer = false;
        json data = json();
        NetworkPackage() {}
        NetworkPackage(bool forServer, json& data) : forServer(forServer), data(data) {}
    };
    using UNetworkPackage = std::unique_ptr<NetworkPackage>;

    class NetworkHelper {
        public:
            UNetworkPackage package = nullptr;

            NetworkHelper() {}

            void sendClientActionsToSocket(SocketBackend* socketBackend, ActionQueue* actionQueue,
                uint32_t thisPlayer, long gameFrameCount) {
                //

                assert(socketBackend);
                assert(actionQueue);
                auto actions_str = getLocalActionsAsString(actionQueue, thisPlayer, gameFrameCount);
                socketBackend->send(actions_str);
            }

            void processReceivedMessage(SocketBackend* socketBackend) {
                package = nullptr;
                auto received = socketBackend->receive();
                if (!received.has_value()) return;
                json j;
                try {
                    j = json::parse(received.value());
                } catch (json::parse_error& e) {
                    LOG_ERROR("Received bad network package")
                    return;
                }
                if (!j.contains(S_NET_MESSAGE_TYPE)) {
                    LOG_ERROR("Network package incorrectly formatted")
                    return;
                }
                auto type = j[S_NET_MESSAGE_TYPE];
                if (type == S_NET_ACTION_FROM_CLIENT) {
                    package = std::make_unique<NetworkPackage>(true, j);
                } else if (type == S_NET_ACTIONS_FROM_SERVER) {
                    package = std::make_unique<NetworkPackage>(false, j);
                } else {
                    LOG_ERROR("Network package has unknown message type: &1", type)
                    return;
                }
            }

            void fillActionQueueFromJson(const json& j, ActionQueue* actionQueue) {
                assert(actionQueue);
                actionQueue->clear();

                if (!j.contains(SA_ACTIONS)) {
                    LOG_ERROR("Network package does not contain actions")
                    return;
                }
                if (!j[SA_ACTIONS].is_array()) {
                    LOG_ERROR("Network package do not contain an action array")
                    return;
                }

                for (const auto& actionJson : j[SA_ACTIONS]) {
                    auto action = std::make_shared<Action>();
                    action->from_json(actionJson);
                    actionQueue->enqueue(action);
                }
            }

            std::string getLocalActionsAsString(ActionQueue* actionQueue, uint32_t thisPlayer, long gameFrameCount) {
                json j;
                to_json_for_network(j, S_NET_ACTION_FROM_CLIENT, actionQueue, thisPlayer, gameFrameCount);
                actionQueue->clear();
                return j.dump();
            }

            void to_json_for_network(json& j, std::string message_type, ActionQueue* actionQueue,
                uint32_t thisPlayer, long gameFrameCount) const {
                //

                assert(actionQueue);
                to_json(j, actionQueue, thisPlayer, gameFrameCount);
                j[S_NET_MESSAGE_TYPE] = message_type;
            }

            void to_json(json& j, ActionQueue* actionQueue, uint32_t thisPlayer, long gameFrameCount) const {
                assert(actionQueue);
                for (const auto& action : actionQueue->getActions()) {
                    json actionJson;
                    action->to_json(actionJson);
                    j[SA_ACTIONS].push_back(actionJson);
                }
                j[SA_PLAYER] = thisPlayer;
                j[SA_FRAME_COUNT] = gameFrameCount;
            }
            
    };

    using UNetworkHelper = std::unique_ptr<NetworkHelper>;

}
