#pragma once

#include <App/layer.hpp>
#include <Game/game.hpp>
#include <Action/action_queue.hpp>


namespace SupDef {

    class GameLayer : public Layer {
        private:
            UGame game;
            USelectionManager selectionManager;
            UActionQueue actionQueue;
            bool blockedByNetwork = false;
            long frameCount = 0;
    
        public:
            GameLayer() {
                priority = PRIORITY_GAME;
            }

            void onAttach() override {
                actionQueue = std::make_unique<ActionQueue>();
                game = std::make_unique<Game>();
                game->setGlobalDispatcher(globalDispatcher);
                game->setActionQueue(actionQueue.get());
                game->initialize();
                selectionManager = std::make_unique<SelectionManager>();
                selectionManager->setGlobalDispatcher(globalDispatcher);
                selectionManager->initialize();

                SUBSCRIBE(GameBlockedByNetworkEvent)
                SUBSCRIBE(ActionsReceivedForGameEvent)
            }
        
            void update(float deltaTime) override {
                if (blockedByNetwork) return;
                assert(game);
                game->update(deltaTime);
                frameCount++;
                game->setFrameCount(frameCount);
                assert(game->getThisPlayer());
                dispatch<GameHasUpdatedEvent>(game->getThisPlayer()->id, frameCount);
            }

            DEFINE_EVENT_CALLBACK(GameBlockedByNetworkEvent) {
                blockedByNetwork = event.blocked;
            }
            
            DEFINE_EVENT_CALLBACK(ActionsReceivedForGameEvent) {
                if (!game) return;
                if (game->isReplay()) return;
                assert(actionQueue);
                assert(event.actionQueue);
                actionQueue->clear();
                actionQueue->getActions() = event.actionQueue->getActions();
            }
            
            Game* getGame() { return game.get(); }
            SelectionManager* getSelectionManager() { return selectionManager.get(); }
            ActionQueue* getActionQueue() { return actionQueue.get(); }

    };
    
}
