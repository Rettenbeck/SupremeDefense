#include <Game/game.hpp>
#include <GUI/include.hpp>

#pragma once


namespace SupDef {

    class Renderer {
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
            EventDispatcher* globalDispatcher = nullptr;

    };

    using URenderer = std::unique_ptr<Renderer>;

}
