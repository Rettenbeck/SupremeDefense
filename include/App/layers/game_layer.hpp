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
            bool blocked = false;
    
        public:
            GameLayer() {}

            void onAttach() override {
                actionQueue = std::make_unique<ActionQueue>();
                game = std::make_unique<Game>();
                game->setGlobalDispatcher(globalDispatcher);
                game->setActionQueue(actionQueue.get());
                game->initialize();
                selectionManager = std::make_unique<SelectionManager>();
                selectionManager->setGlobalDispatcher(globalDispatcher);
                selectionManager->initialize();

                SUBSCRIBE_BEGIN(globalDispatcher, GameBlockedEvent)
                // globalDispatcher->subscribe<GameBlockedEvent>([this](const SupDef::Events& events) {
                //     for (const auto& event : events) {
                //         const auto& typedEvent = static_cast<const UpdateCommandEvent&>(*event);
                    blocked = typedEvent.blocked;
                //     }
                // });
                SUBSCRIBE_END
            }
        
            void update(float deltaTime) override {
                if (blocked) return;
                game->update(deltaTime);
            }
            
            Game* getGame() { return game.get(); }
            SelectionManager* getSelectionManager() { return selectionManager.get(); }
            ActionQueue* getActionQueue() { return actionQueue.get(); }

    };
    
}
