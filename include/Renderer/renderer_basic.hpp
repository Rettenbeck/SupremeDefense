#pragma once

#include <Renderer/renderer.hpp>

#include <SFML/Main.hpp>
#include <SFML/Graphics.hpp>

#include <imgui.h>
#include <imgui-SFML.h>


namespace SupDef {

    using RSelectable  = std::tuple<Entity*, Entity*, BoundingBox*, float, float>;
    using RSelectables = std::vector<RSelectable>;

    using UTexture = std::unique_ptr<sf::Texture>;
    using TextureMap = std::unordered_map<std::string, UTexture>;

    enum class RCommandMode {
        NONE,
        MOVE,
        FRAME,
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

            DEFINE_EVENT_CALLBACK(CommandToRenderEvent);
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
            void renderEntityWithInfluence(PositionComponent* pos, CollisionComponent* col);
            void renderSelectedUnits();
            void renderSelectedUnit(EntityID entityID);
            void renderVirtualEntity();
            void drawHealthBar(Entity* entity, PositionComponent* pos, CollisionComponent* col, int hbWidth, int hbHeight);
            void drawHealthBar(float x, float y, float width, float height, long health, long health_max);
            void renderMaps(EntityManager* entityManager);
            void renderMap(_EntMapTiles map);

            void drawSprites(EntityManager* entityManager);
            void drawSprite(Entity* entity, PositionComponent* pos, GraphicComponent* gra);
            void handleAnimation(Entity* entity, sf::Sprite& sprite, PositionComponent* pos);
            void handleRotation(Entity* entity, sf::Sprite& sprite, PositionComponent* pos, bool centered);
            sf::Texture* getTexture(GraphicComponent* graphic);
            sf::Texture* getTexture(const std::string& filename);
            
            void showDebug();
            void renderGui();
            void drawElement(GuiElement* element);
            void drawPanel(GuiElementStyle style, float x, float y, float width, float height);
            void drawLabel(GuiElementStyle style, float x, float y, std::string text);
            void drawButton(GuiElementStyle style, float x, float y, float width, float height, std::string text);
            void drawCheckbox(GuiElementStyle style, float x, float y, std::string text, bool* checked);
            void drawCheckbox(GuiCheckbox* checkbox);
            void drawTable(GuiTable* table);
            void drawCooldownOverlay(float x, float y, float width, float height, Cooldown current, Cooldown max);
            void drawCooldownOverlay(float x, float y, float width, float height, float perc);
            void addClickHandling(GuiElement* element);
            float getRowHeight(GuiElementRow& row);

            void drawRect(float x, float y, float width, float height, ColorData cData);
            void drawCircle(float x, float y, float r, ColorData cData);
            void drawLine(float x1, float y1, float x2, float y2, sf::Color color);
            void drawLine1D(float x1, float y1, float x2, float y2, sf::Color color);

            void drawSelection(float x, float y, float width, float height, ColorData cData);

            sf::Clock deltaClock;
            std::unique_ptr<sf::RenderWindow> window;

            sf::View gameView;
            sf::View guiView;
            bool dontSetPosition = false;

            float currentZoom = 1.0;
            float moveAmount = 10.0;
            bool keyL = false, keyR = false, keyU = false, keyD = false;
            RCommandMode commandMode = RCommandMode::NONE;
            CommandID currentCommand = NO_COMMAND;
            Entity* virtualEntity = nullptr;

            EntityID currentMap = NO_ENTITY;
            RSelectables selectables;

            TextureMap textureMap;

            bool debugMode = false;
            bool drawTiles = false;

            //std::unique_ptr<GUI::Root> gui;
            //std::unique_ptr<sf::Font> font;

    };

}
