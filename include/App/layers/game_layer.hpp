#include <App/layer.hpp>
#include <Game/game.hpp>
#include <Action/action_queue.hpp>

#pragma once


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

                SUBSCRIBE_BEGIN(globalDispatcher, GameBlockedByNetworkEvent)
                    blockedByNetwork = typedEvent.blocked;
                SUBSCRIBE_END
            }
        
            void update(float deltaTime) override {
                if (blockedByNetwork) return;
                assert(game);
                assert(globalDispatcher);
                game->update(deltaTime);
                frameCount++;
                assert(game->getThisPlayer());
                globalDispatcher->dispatch<GameHasUpdatedEvent>(game->getThisPlayer()->id, frameCount);
            }
            
            Game* getGame() { return game.get(); }
            SelectionManager* getSelectionManager() { return selectionManager.get(); }
            ActionQueue* getActionQueue() { return actionQueue.get(); }

    };
    
}
