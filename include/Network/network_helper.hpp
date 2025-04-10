#include <Util/util.hpp>
#include <App/constants.hpp>
#include <Action/action_queue.hpp>
#include <Network/socket_backend.hpp>
#include <App/Log/logger.hpp>

#pragma once


namespace SupDef {

    enum class NetworkMessageType {
        ActionFromClient, ActionsFromServer, Chat, Assets, Undefined
    };

    struct NetworkPackage {
        bool forServer = false;
        json data = json();
        NetworkMessageType type = NetworkMessageType::Undefined;
        NetworkPackage() {}
        NetworkPackage(bool forServer, json& data) : forServer(forServer), data(data) {}
        NetworkPackage(bool forServer, json& data, NetworkMessageType type)
        : forServer(forServer), data(data), type(type) {}
    };
    using UNetworkPackage = std::unique_ptr<NetworkPackage>;

    class NetworkHelper {
        public:
            UNetworkPackage package = nullptr;

            NetworkHelper() {}

            void sendJsonToSocket(SocketBackend* socketBackend, json& j) {
                assert(socketBackend);
                socketBackend->send(j.dump(2));
            }

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
                LOG(Info, "Message received: &1", received.value())
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
                    setPackage(true, j, NetworkMessageType::ActionFromClient);
                } else if (type == S_NET_ACTIONS_FROM_SERVER) {
                    setPackage(false, j, NetworkMessageType::ActionsFromServer);
                } else if (type == S_NET_CHAT) {
                    setPackage(false, j, NetworkMessageType::Chat);
                } else if (type == S_NET_ASSETS) {
                    setPackage(false, j, NetworkMessageType::Assets);
                } else {
                    LOG_ERROR("Network package has unknown message type: &1", type)
                    return;
                }
            }

            void setPackage(bool forServer, json j, NetworkMessageType type) {
                package = std::make_unique<NetworkPackage>(forServer, j, type);
            }

            void fillActionQueueFromJson(const json& j, ActionQueue* actionQueue, long frameCount) {
                assert(actionQueue);
                actionQueue->clear();

                JSON_CONTAINS_WITH_MSG_AND_RETURN(j, SA_ACTIONS, "Network package does not contain actions");
                JSON_CONTAINS_WITH_MSG_AND_RETURN(j, SA_FRAME_COUNT, "Network package does not contain frame count");
                CHECK_CONDITION_WITH_MSG_AND_RETURN(!j[SA_ACTIONS].is_array(), "Network package do not contain an action array")
                CHECK_CONDITION_WITH_MSG_AND_RETURN((long) j[SA_FRAME_COUNT] != frameCount, "Network package contains different frame count")

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
