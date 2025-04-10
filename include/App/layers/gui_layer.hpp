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

            GuiLayer() {
                priority = PRIORITY_GUI;
            }
        
            void onAttach() override {
                SUBSCRIBE_BEGIN(globalDispatcher, WindowResizeEvent)
                    if (guiManager) guiManager->resize(typedEvent.width, typedEvent.height);
                SUBSCRIBE_END
            }
        
            void onDetach() override {
                // Cleanup logic if needed
            }
        
            template <typename T>
            T* setGuiManager() {
                // static_assert(std::is_base_of<GuiManager, T>, "T must be derived from GuiManager");
                if (!guiManager || !dynamic_cast<T*>(guiManager.get())) {
                    guiManager = std::make_unique<T>();
                    assert(globalDispatcher);
                    globalDispatcher->dispatch<UpdateAppEvent>();
                }
                assert(guiManager);
                guiManager->setGlobalDispatcher(globalDispatcher);
                return dynamic_cast<T*>(guiManager.get());
            }

            void update(float deltaTime) override {
                if (game) {
                    auto ptr = setGuiManager<GuiManagerGame>();
                    ptr->setSelectionManager(selectionManager);
                    ptr->setGame(game);
                    // ptr->update(deltaTime);
                } else {
                    auto ptr = setGuiManager<GuiManagerNetwork>();
                    // ptr->update(deltaTime);
                }
                guiManager->update(deltaTime);
            }
        
            GuiManager* getGuiManager() const { return guiManager.get(); }

            void setGame(Game* game_) { game = game_; }
            void setSelectionManager(SelectionManager* selectionManager_) { selectionManager = selectionManager_; }

    };
    
}
