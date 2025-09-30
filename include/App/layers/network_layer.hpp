#pragma once

#include <App/layer.hpp>
#include <Action/action_queue.hpp>
#include <Network/include.hpp>


namespace SupDef {

    enum class NetworkStatus {
        Initial, Listening, Connected, Completed
    };

    class NetworkLayer : public Layer {
        private:
            USocketBackend socketBackend;
            UNetworkPlayerTracker networkPlayerTracker;
            UActionQueue actionQueue;
            UActionQueue actionQueueForGame;
            NetworkStatus status = NetworkStatus::Initial;
            bool isServer = false;
            bool hasGameUpdated = false;
            long gameFrameCount = 0;

        public:
            NetworkLayer() {
                priority = PRIORITY_NETWORK;
                networkPlayerTracker = std::make_unique<NetworkPlayerTracker>();
                socketBackend = std::make_unique<SFMLSocketBackend>();
                actionQueue = std::make_unique<ActionQueue>();
                actionQueueForGame = std::make_unique<ActionQueue>();
            }
        
            void onAttach() override {
                SUBSCRIBE_BEGIN(globalDispatcher, GameHasUpdatedEvent)
                    assert(networkPlayerTracker);
                    networkPlayerTracker->setThisPlayer(typedEvent.thisPlayer);
                    hasGameUpdated = true;
                    gameFrameCount = typedEvent.frameCount;
                SUBSCRIBE_END
                SUBSCRIBE_BEGIN(globalDispatcher, SendPlayerListEvent)
                    assert(networkPlayerTracker);
                    networkPlayerTracker->resetPlayerList(typedEvent.playerList);
                SUBSCRIBE_END
            }
        
            void update(float deltaTime) override {
                // assert()
                // switch(status) {
                //     case NetworkStatus::Initial:
                //         break;
                //     case NetworkStatus::Listening:
                //         checkForClients();
                //         break;
                //     case NetworkStatus::Connected:
                //     case NetworkStatus::Completed:
                //         send();
                //         receive();
                //         break;
                //     default:
                //         assert(false);
                // }
            }

            SocketBackend* getSocketBackend() { return socketBackend.get(); }

            ActionQueue* getActionQueue() { return actionQueue.get(); }

    };    
    
}
