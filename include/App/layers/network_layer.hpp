#include <App/layer.hpp>
#include <Action/action_queue.hpp>
#include <Network/include.hpp>

#pragma once


namespace SupDef {

    class NetworkLayer : public Layer {
        private:
            USocketBackend socketBackend;
            UNetworkHelper networkHelper;
            UNetworkPlayerTracker npt;
            UActionQueue actionQueue;
            UActionQueue actionQueueForGame;
            bool isServer = false;
            bool hasGameUpdated = false;
            long gameFrameCount = 0;

        public:
            NetworkLayer() {
                npt = std::make_unique<NetworkPlayerTracker>();
                socketBackend = std::make_unique<SFMLSocketBackend>();
                networkHelper = std::make_unique<NetworkHelper>();
                actionQueue = std::make_unique<ActionQueue>();
                actionQueueForGame = std::make_unique<ActionQueue>();
            }
        
            void onAttach() override {
                SUBSCRIBE_BEGIN(globalDispatcher, GameHasUpdatedEvent)
                    assert(npt);
                    npt->setThisPlayer(typedEvent.thisPlayer);
                    hasGameUpdated = true;
                    gameFrameCount = typedEvent.frameCount;
                SUBSCRIBE_END
                SUBSCRIBE_BEGIN(globalDispatcher, SendPlayerListEvent)
                    assert(npt);
                    npt->resetPlayerList(typedEvent.playerList);
                SUBSCRIBE_END
            }
        
            void update(float deltaTime) override {
                send();
            }

            void send() {
                if (!hasGameUpdated) return;
                assert(actionQueueForGame);
                assert(networkHelper);
                assert(npt);
                globalDispatcher->dispatch<GameBlockedByNetworkEvent>(true);

                if (isServer) {
                    // if (hasGameUpdated) send all actions to socket
                } else {
                    networkHelper->sendClientActionsToSocket(
                        socketBackend.get(), actionQueue.get(), npt->getThisPlayer(), gameFrameCount
                    );
                }

                hasGameUpdated = false;
            }

            void receive() {
                assert(networkHelper);
                checkReceivedMessage();
                if (!networkHelper->package) return;

                if (isServer) {
                    //
                } else {
                    networkHelper->fillActionQueueFromJson(networkHelper->package->data, actionQueueForGame.get());
                    globalDispatcher->dispatch<ReceivedActionsFromServerEvent>(actionQueueForGame.get());
                    globalDispatcher->dispatch<GameBlockedByNetworkEvent>(false);
                }
            }

            void checkReceivedMessage() {
                assert(networkHelper);
                networkHelper->processReceivedMessage(socketBackend.get());
                if (networkHelper->package) {
                    if (networkHelper->package->forServer != isServer) {
                        LOG_ERROR("Received unfitting message")
                        networkHelper->package = nullptr;
                    }
                }
            }

            ActionQueue* getActionQueue() { return actionQueue.get(); }

    };    
    
}
