#include <App/layer.hpp>
#include <GUI/include.hpp>
#include <Renderer/renderer_basic.cpp>

#pragma once


namespace SupDef {

    class RenderLayer : public Layer {
        private:
            URenderer renderer = nullptr;
            GuiManager* gui = nullptr;
            Game* game = nullptr;

        public:
            RenderLayer() {
                renderer = std::make_unique<RendererBasic>();
            }
        
            void onAttach() override {
                if(!renderer) return;
                renderer->globalDispatcher = globalDispatcher;
            }
        
            void onStart() override {
                if(!renderer) return;
                renderer->start();
            }
        
            void update(float deltaTime) override {
                renderer->game = game;
                renderer->gui = gui;
                bool end = false;

                if(!renderer->draw()) end = true;
                if(!renderer->poll()) end = true;

                if(end) {
                    renderer->end();
                    globalDispatcher->dispatch<GameEndEvent>();
                }
            }

            void setWindowSize(unsigned width, unsigned height) {
                if (renderer) {
                    renderer->width  = width ;
                    renderer->height = height;
                }
            }

            // Game* getGame() { return game; }
            // GuiManager* getGuiManager() const { return gui; }

            void setGame(Game* game_) { game = game_; }
            void setGuiManager(GuiManager* gui_) { gui = gui_; }

    };    
    
}
