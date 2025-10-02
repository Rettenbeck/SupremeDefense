#pragma once

#include <App/layer.hpp>
#include <GUI/include.hpp>
#include <Renderer/renderer_basic.hpp>

#ifdef INCLUDE_CPP
    #include <Renderer/renderer_basic.cpp>
#endif


namespace SupDef {

    class RenderLayer : public Layer {
        private:
            URenderer renderer = nullptr;
            GuiManager* gui = nullptr;
            Game* game = nullptr;

        public:
            RenderLayer() {
                priority = PRIORITY_RENDER;
                renderer = std::make_unique<RendererBasic>();
            }
        
            void onAttach() override {
                if(!renderer) return;
                renderer->setGlobalDispatcher(globalDispatcher);
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
                    dispatch<GameEndEvent>();
                }
            }

            void setWindowSize(unsigned width, unsigned height) {
                if (renderer) {
                    renderer->width  = width ;
                    renderer->height = height;
                }
            }

            void setFramecountData(float fc_total, float fc_renderer, float fc_game) {
                if (renderer) {
                    renderer->framecountTotal = fc_total;
                    renderer->framecountRenderer = fc_renderer;
                    renderer->framecountGame = fc_game;
                }
            }

            void setGame(Game* game_) { game = game_; }
            void setGuiManager(GuiManager* gui_) { gui = gui_; }

    };    
    
}
