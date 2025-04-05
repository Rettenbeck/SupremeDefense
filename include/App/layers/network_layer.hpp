#include <App/layer.hpp>
#include <Action/action_queue.hpp>
#include <Network/include.hpp>

#pragma once


namespace SupDef {

    class NetworkLayer : public Layer {
        private:
            USocketBackend socketBackend;
            UNetworkManager networkManager;
            UActionQueue actionQueue;
            UActionQueue actionQueueServer;
            bool isServer = false;
            bool hasGameUpdated = false;
            long gameFrameCount = 0;
            uint32_t thisPlayer = 0;
            std::vector<uint32_t> players;

        public:
            NetworkLayer() {
                socketBackend = std::make_unique<SFMLSocketBackend>();
                networkManager = std::make_unique<NetworkManager>();
                actionQueue = std::make_unique<ActionQueue>();
                actionQueueServer = std::make_unique<ActionQueue>();
            }
        
            void onAttach() override {
                SUBSCRIBE_BEGIN(globalDispatcher, GameHasUpdatedEvent)
                    hasGameUpdated = true;
                    thisPlayer = typedEvent.thisPlayer;
                    gameFrameCount = typedEvent.frameCount;
                SUBSCRIBE_END
            }
        
            void update(float deltaTime) override {
                assert(actionQueueServer);
                assert(networkManager);
                checkReceivedMessage();

                if (isServer) {
                    // if (hasGameUpdated) send all actions to socket
                } else {
                    if (hasGameUpdated) {
                        networkManager->sendClientActionsToSocket(
                            socketBackend.get(), actionQueue.get(), thisPlayer, gameFrameCount
                        );
                    }
                }

                hasGameUpdated = false;
            }

            void checkReceivedMessage() {
                assert(networkManager);
                networkManager->hasReceivedMessage(socketBackend.get());
                if (networkManager->package) {
                    if (networkManager->package->forServer != isServer) {
                        Logger::getInstance().addMessage(MessageType::Error, "Received unfitting message");
                        networkManager->package = nullptr;
                    }
                }
            }

            ActionQueue* getActionQueue() { return actionQueue.get(); }

    };    
    
}
