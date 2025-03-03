#include <Renderer/renderer.cpp>

#include <SFML/Main.hpp>
#include <SFML/Graphics.hpp>

#include <imgui.h>
#include <imgui-SFML.h>

#pragma once


namespace SupDef {

    struct ColorData {
        ColorData() { }
        ColorData(sf::Color fillColor_, sf::Color outlineColor_, float outlineThickness_) {
            fillColor = fillColor_;
            outlineColor = outlineColor_;
            outlineThickness = outlineThickness_;
        }
        sf::Color fillColor;
        sf::Color outlineColor;
        float outlineThickness;
    };

    class RendererBasic: public Renderer {
        public:

            RendererBasic();
            ~RendererBasic();
            
            // void initialize();
            void start();
            void end();

            void buildGUI();

            bool poll();
            bool draw();

            void zoom(float factor);
            void move(float offsetX, float offsetY);

            void subscribeToEvents();

            void renderGame();
            void renderCollisionGrid();
            void renderEntitiesWithCollision(EntityManager* entityManager);
            void renderMaps(EntityManager* entityManager);
            void renderMap(_EntMapTiles map);
            
            void renderGui();
            void drawPanel(GuiElementStyle style, float x, float y, float width, float height);
            void drawLabel(GuiElementStyle style, float x, float y, std::string text);
            void drawButton(GuiElementStyle style, float x, float y, float width, float height, std::string text);

            void drawRect(float x, float y, float width, float height, ColorData cData);
            void drawCircle(float x, float y, float r, ColorData cData);
            void drawLine(float x1, float y1, float x2, float y2, sf::Color color);

            sf::Clock deltaClock;
            std::unique_ptr<sf::RenderWindow> window;

            sf::View gameView;
            sf::View guiView;

            float currentZoom = 1.0;
            bool keyL = false, keyR = false, keyU = false, keyD = false;
            int commandMode = 0;

            //std::unique_ptr<GUI::Root> gui;
            //std::unique_ptr<sf::Font> font;

    };

}
