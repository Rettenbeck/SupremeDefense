#pragma once

#include <App/layers/game_layer.hpp>
#include <App/layers/replay_layer.hpp>
#include <App/layers/network_layer.hpp>
#include <EventDispatcher/events.hpp>
#include <Action/include.hpp>


namespace SupDef {

    class ActionRouter : public Layer {
        private:
            GameLayer* gameLayer = nullptr;
            ReplayLayer* replayLayer = nullptr;
            NetworkLayer* networkLayer = nullptr;
        
            bool isMultiplayer = false;
            bool isReplay = false;
        
        public:
            void prepare(GameLayer* game, ReplayLayer* replay, NetworkLayer* network) {
                gameLayer = game;
                replayLayer = replay;
                networkLayer = network;
                isMultiplayer = (network != nullptr);
                isReplay = (replay == nullptr);
            }

            ActionRouter() {
                priority = PRIORITY_ACTION_ROUTER;
            }

            void onAttach() override {
                SUBSCRIBE(ActionCreatedEvent)
                SUBSCRIBE(ReceivedActionsFromServerEvent)
            }
        
            void handleAction(SAction action) {
                auto& logger = Logger::getInstance();
                logger.addMessage(MessageType::Info, "Action created");

                if (isMultiplayer && networkLayer) {
                    logger.addMessage(MessageType::Info, "  Sent to network");
                    networkLayer->getActionQueue()->enqueue(action);
                } else {
                    logger.addMessage(MessageType::Info, "  Sent to game");
                    forwardActionToGame(action);
                }
        
                if (replayLayer && !isReplay) {
                    logger.addMessage(MessageType::Info, "  Sent to replay");
                    replayLayer->getActionQueue()->enqueue(action);
                }
            }

            void forwardActionToGame(SAction action) {
                gameLayer->getActionQueue()->enqueue(action);
            }
        
            void forwardActionsToGame(ActionQueue* actionQueue) {
                assert(actionQueue);
                for(auto action : actionQueue->getActions()) {
                    gameLayer->getActionQueue()->enqueue(action);
                }
            }
        
            DEFINE_EVENT_CALLBACK(ActionCreatedEvent) {
                handleAction(event.action);
            }

            DEFINE_EVENT_CALLBACK(ReceivedActionsFromServerEvent) {
                forwardActionsToGame(event.actionQueue);
            }

            void update(float deltaTime) override {
                // Process any queued events or additional logic if necessary
            }

    };    
    
}
