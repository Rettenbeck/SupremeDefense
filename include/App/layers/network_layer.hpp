#include <App/layer.hpp>
#include <Action/action_queue.hpp>
#include <Network/include.hpp>

#pragma once


namespace SupDef {

    class NetworkLayer : public Layer {
        private:
            USocketBackend socketBackend;
            UActionQueue actionQueue;
            bool isServer = false;
            bool hasGameUpdated = false;
            long gameFrameCount = 0;
            uint32_t thisPlayer = 0;

        public:
            NetworkLayer() {
                // socketBackend = std::make_unique<SFMLSocketBackend>();
                actionQueue = std::make_unique<ActionQueue>();
            }
        
            void onAttach() override {
                SUBSCRIBE_BEGIN(globalDispatcher, GameHasUpdatedEvent)
                    hasGameUpdated = true;
                SUBSCRIBE_END
                SUBSCRIBE_BEGIN(globalDispatcher, UpdateGameFrameCountEvent)
                    gameFrameCount = typedEvent.frameCount;
                SUBSCRIBE_END
                SUBSCRIBE_BEGIN(globalDispatcher, UpdateThisPlayerEvent)
                    thisPlayer = typedEvent.thisPlayer;
                SUBSCRIBE_END
            }
        
            void update(float deltaTime) override {
                if (isServer) {
                    // check whether socket has received messages from another client
                    // if (hasGameUpdated) send all actions to socket
                } else {
                    // check whether socket has received messages from server
                    // if (hasGameUpdated) send this player's action to socket
                }

                hasGameUpdated = false;
            }

            void to_json(json& j) const {
                for (const auto& action : actionQueue->getActions()) {
                    json actionJson;
                    action->to_json(actionJson);
                    j[SA_ACTIONS].push_back(actionJson);
                }
                j[SA_PLAYER] = thisPlayer;
                j[SA_FRAME_COUNT] = gameFrameCount;
            }

            ActionQueue* getActionQueue() { return actionQueue.get(); }

    };    
    
}
