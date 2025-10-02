#pragma once

#include <Game/game.hpp>
#include <GUI/include.hpp>


namespace SupDef {

    class Renderer : public Listener {
        public:

            Renderer();
            virtual ~Renderer();

            // virtual void initialize();
            virtual void start();
            virtual void end();

            virtual bool poll();
            virtual bool draw();
            
            unsigned width = 1280, height = 720;
            Game* game = nullptr;
            GuiManager* gui = nullptr;

            float framecountTotal = 0.0, framecountRenderer = 0.0, framecountGame = 0.0;

    };

    using URenderer = std::unique_ptr<Renderer>;

}
