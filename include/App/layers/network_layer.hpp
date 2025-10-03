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
                socketBackend->setGlobalDispatcher(globalDispatcher);
                socketBackend->initialize();
                socketBackend->start(discoveryPortInitial, connectionPortInitial);

                SUBSCRIBE(RequestServerOpenEvent)
                SUBSCRIBE(RequestOpenServerCloseEvent)

                // SUBSCRIBE_BEGIN(globalDispatcher, GameHasUpdatedEvent)
                //     assert(networkPlayerTracker);
                //     networkPlayerTracker->setThisPlayer(typedEvent.thisPlayer);
                //     hasGameUpdated = true;
                //     gameFrameCount = typedEvent.frameCount;
                // SUBSCRIBE_END
                // SUBSCRIBE_BEGIN(globalDispatcher, SendPlayerListEvent)
                //     assert(networkPlayerTracker);
                //     networkPlayerTracker->resetPlayerList(typedEvent.playerList);
                // SUBSCRIBE_END
            }
        
            void update(float deltaTime) override {
                assert(socketBackend);
                socketBackend->update();
            }

            DEFINE_EVENT_CALLBACK(RequestServerOpenEvent) {
                assert(socketBackend);
                if (status != NetworkStatus::Initial) {
                    dispatch<RequestServerOpenAnswerEvent>(false, "Cannot open server; wrong network layer status");
                    return;
                }
                auto result = socketBackend->openServer(connectionPortInitial);
                if (result.ok) status = NetworkStatus::Listening;
                dispatch<RequestServerOpenAnswerEvent>(result.ok, result.error);
                if (result.ok) std::cout << "Server open\n";
            }

            DEFINE_EVENT_CALLBACK(RequestOpenServerCloseEvent) {
                assert(socketBackend);
                if (status != NetworkStatus::Listening) {
                    dispatch<RequestOpenServerCloseAnswerEvent>(false, "Cannot close server; wrong network layer status");
                    return;
                }
                socketBackend->closeServer();
                status = NetworkStatus::Initial;
                dispatch<RequestOpenServerCloseAnswerEvent>(true, "");
                std::cout << "Server closed\n";
            }

            // void onRequestServerOpenEvent(bool ok, std::string message) {
            //     //
            // }

            SocketBackend* getSocketBackend() { return socketBackend.get(); }

            ActionQueue* getActionQueue() { return actionQueue.get(); }

    };    
    
}
