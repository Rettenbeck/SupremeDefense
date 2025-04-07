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
                receive();
            }

            void send() {
                if (!hasGameUpdated) return;
                assert(actionQueueForGame);
                assert(networkHelper);
                assert(npt);
                globalDispatcher->dispatch<GameBlockedByNetworkEvent>(true);

                if (isServer) {
                    // Nothing to do -> handled in the receive logic
                } else {
                    networkHelper->sendClientActionsToSocket(
                        socketBackend.get(), actionQueue.get(), npt->getThisPlayer(), gameFrameCount
                    );
                }

                hasGameUpdated = false;
            }

            bool receive() {
                assert(networkHelper);
                assert(npt);
                checkReceivedMessage();
                if (!networkHelper->package) return false;

                if (isServer) {
                    npt->addPlayerMessage(networkHelper->package->data);
                    if (npt->complete()) {
                        json j;
                        npt->to_json(j);
                        networkHelper->sendJsonToSocket(socketBackend.get(), j);
                        receiveActionQueue(j);
                    }
                } else {
                    receiveActionQueue(networkHelper->package->data);
                }
                
                return true;
            }

            void receiveActionQueue(json& j) {
                networkHelper->fillActionQueueFromJson(j, actionQueueForGame.get(), gameFrameCount);
                globalDispatcher->dispatch<ReceivedActionsFromServerEvent>(actionQueueForGame.get());
                globalDispatcher->dispatch<GameBlockedByNetworkEvent>(false);
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

            bool startNetworkGameAsServer(unsigned short port) {
                assert(socketBackend);
                if(!socketBackend->startAsServer(port)) {
                    LOG_ERROR("Starting as server failed");
                    return false;
                }
                isServer = true;
                return true;
            }

            bool startNetworkGameAsClient(const std::string& ip, unsigned short port) {
                assert(socketBackend);
                if(!socketBackend->startAsClient(ip, port)) {
                    LOG_ERROR("Starting as client failed");
                    return false;
                }
                isServer = false;
                return true;
            }

            ActionQueue* getActionQueue() { return actionQueue.get(); }

    };    
    
}
