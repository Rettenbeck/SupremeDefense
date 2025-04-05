#include <App/layer.hpp>
#include <GUI/include.hpp>
#include <Game/game.hpp>

#pragma once


namespace SupDef {

    class GuiLayer : public Layer {
        private:
            UGuiManager guiManager = nullptr;
            SelectionManager* selectionManager = nullptr;
            Game* game = nullptr;

        public:

            GuiLayer() { }
        
            void onAttach() override {
                guiManager = std::make_unique<GuiManager>();
                
                SUBSCRIBE_BEGIN(globalDispatcher, WindowResizeEvent)
                    guiManager->resize(typedEvent.width, typedEvent.height);
                SUBSCRIBE_END
            }
        
            void onDetach() override {
                // Cleanup logic if needed
            }
        
            void update(float deltaTime) override {
                if (guiManager) {
                    guiManager->setGlobalDispatcher(globalDispatcher);
                    guiManager->setSelectionManager(selectionManager);
                    guiManager->setGame(game);
                    guiManager->update(deltaTime);
                }
            }
        
            GuiManager* getGuiManager() const { return guiManager.get(); }

            void setGame(Game* game_) { game = game_; }
            void setSelectionManager(SelectionManager* selectionManager_) { selectionManager = selectionManager_; }

    };
    
}
