#include <Renderer/renderer.cpp>

#include <SFML/Main.hpp>
#include <SFML/Graphics.hpp>

#include <imgui.h>
#include <imgui-SFML.h>

#pragma once


namespace SupDef {

    using RSelectable  = std::tuple<Entity*, Entity*, BoundingBox*, float, float>;
    using RSelectables = std::vector<RSelectable>;

    enum class RCommandMode {
        NONE,
        MOVE,
        BUILD
    };

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
            
            void start();
            void end();

            void buildGUI();

            bool poll();
            bool draw();

            void doScrolling();
            void zoom(float factor);
            void move(float offsetX, float offsetY);
            void onMouseClick(MouseClick button);
            EntityID mouseToMap();

            sf::Vector2i getMousePos();
            sf::Vector2f getMousePosWorld();

            void prepareSelectableList();
            std::tuple<Entity*, EntityID> getUnitInSpot(float worldX, float worldY);
            EntityID getMapByCoords(float worldX, float worldY);

            void trigger(int i);
            void subscribeToEvents();

            void onCommandToRender(const CommandToRenderEvent& event);
            void resetCommand();
            void updateCommand();
            void processCommand(Entity* asset, const json &j);
            void processCommandMessage(Entity* asset, std::string msg, const json &j);
            bool addPositionData(json& j);
            bool addVirtualEntityData(json& j);

            void renderGame();
            void renderCollisionGrid();
            void renderEntitiesWithCollision(EntityManager* entityManager);
            void renderEntityWithCollision(PositionComponent* pos, CollisionComponent* col, bool drawBB);
            void renderSelectedUnits();
            void renderSelectedUnit(EntityID entityID);
            void renderVirtualEntity();
            void renderMaps(EntityManager* entityManager);
            void renderMap(_EntMapTiles map);
            
            void showDebug();
            void renderGui();
            void drawPanel(GuiElementStyle style, float x, float y, float width, float height);
            void drawLabel(GuiElementStyle style, float x, float y, std::string text);
            void drawButton(GuiElementStyle style, float x, float y, float width, float height, std::string text);

            void drawRect(float x, float y, float width, float height, ColorData cData);
            void drawCircle(float x, float y, float r, ColorData cData);
            void drawLine(float x1, float y1, float x2, float y2, sf::Color color);
            void drawLine1D(float x1, float y1, float x2, float y2, sf::Color color);

            void drawSelection(float x, float y, float width, float height, ColorData cData);

            sf::Clock deltaClock;
            std::unique_ptr<sf::RenderWindow> window;

            sf::View gameView;
            sf::View guiView;

            float currentZoom = 1.0;
            float moveAmount = 10.0;
            bool keyL = false, keyR = false, keyU = false, keyD = false;
            RCommandMode commandMode = RCommandMode::NONE;
            CommandID currentCommand = NO_COMMAND;
            Entity* virtualEntity = nullptr;

            EntityID currentMap = NO_ENTITY;
            RSelectables selectables;

            bool debugMode = false;
            bool drawTiles = true;

            //std::unique_ptr<GUI::Root> gui;
            //std::unique_ptr<sf::Font> font;

    };

}
