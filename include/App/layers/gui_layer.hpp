#pragma once

#include <App/layer.hpp>
#include <GUI/include.hpp>
#include <Game/game.hpp>


namespace SupDef {

    class GuiLayer : public Layer {
        private:
            UGuiManager guiManager = nullptr;
            UPageManager pageManager = nullptr;
            SelectionManager* selectionManager = nullptr;
            SocketBackend* socketBackend = nullptr;
            Game* game = nullptr;

        public:
            GuiLayer() {
                priority = PRIORITY_GUI;
                guiManager = std::make_unique<GuiManager>();
                pageManager = std::make_unique<PageManager>();
                pageManager->setGuiManager(guiManager.get());
            }
        
            void onAttach() override {
                SUBSCRIBE_BEGIN(globalDispatcher, WindowResizeEvent)
                    if (guiManager) guiManager->resize(typedEvent.width, typedEvent.height);
                SUBSCRIBE_END
                
                guiManager ->setGlobalDispatcher(globalDispatcher);
                pageManager->setGlobalDispatcher(globalDispatcher);
                guiManager ->initialize();
                pageManager->initialize();
            }
        
            void onDetach() override {
                // Cleanup logic if needed
            }
        
            void update(float deltaTime) override {
                guiManager->setSelectionManager(selectionManager);
                pageManager->setGame(game);
                pageManager->setSelectionManager(selectionManager);
                pageManager->setSocketBackend(socketBackend);
                guiManager->update(deltaTime);
                pageManager->update(deltaTime);
            }
        
            GuiManager* getGuiManager() const { return guiManager.get(); }

            void setGame(Game* game_) { game = game_; }
            void setSelectionManager(SelectionManager* selectionManager_) { selectionManager = selectionManager_; }
            void setSocketBackend(SocketBackend* socketBackend_) { socketBackend = socketBackend_; }

    };
    
}
