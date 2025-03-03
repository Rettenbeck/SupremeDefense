#include <Renderer/renderer_basic.hpp>

#pragma once


namespace SupDef {

    RendererBasic::RendererBasic() { }
    
    RendererBasic::~RendererBasic() {
        ImGui::SFML::Shutdown();
    }

    void RendererBasic::start() {
        unsigned w, h;
        w = width;
        h = height;
        window = std::make_unique<sf::RenderWindow>(sf::VideoMode({width, height}), "Supreme");
        auto initOk = ImGui::SFML::Init(*(window.get()));

        gameView = window->getDefaultView();
        guiView  = window->getDefaultView();

        //font = std::make_unique<sf::Font>();
        //font->loadFromFile("arial.ttf");
        assert(globalDispatcher);
        globalDispatcher->dispatch<WindowResizeEvent>(width, height);
    }

    void RendererBasic::end() {
        if(window->isOpen()) window->close();
    }
    
    bool RendererBasic::poll() {
        if(!window->isOpen()) { return false; }

        while (const std::optional<sf::Event> event = window->pollEvent()) {
            if(event.has_value()) { ImGui::SFML::ProcessEvent(*(window.get()), event.value()); }

            if(event->is<sf::Event::Closed>()) {
                window->close();
                return false;
            }

            if(event.has_value()) {
                if (const auto* resizedEvent = event->getIf<sf::Event::Resized>()) {
                    globalDispatcher->dispatch<WindowResizeEvent>(resizedEvent->size.x, resizedEvent->size.y);
                }

                if (const auto* scrollEvent = event->getIf<sf::Event::MouseWheelScrolled>()) {
                    if (scrollEvent->delta > 0)
                        zoom(0.9f);
                    else
                        zoom(1.1f);
                }
        
                if (const auto* keyEvent = event->getIf<sf::Event::KeyPressed>()) {
                    if (keyEvent->code == sf::Keyboard::Key::Left)  keyL = true;
                    if (keyEvent->code == sf::Keyboard::Key::Right) keyR = true;
                    if (keyEvent->code == sf::Keyboard::Key::Up)    keyU = true;
                    if (keyEvent->code == sf::Keyboard::Key::Down)  keyD = true;
                }

                if (const auto* keyEvent = event->getIf<sf::Event::KeyReleased>()) {
                    if (keyEvent->code == sf::Keyboard::Key::Left)  keyL = false;
                    if (keyEvent->code == sf::Keyboard::Key::Right) keyR = false;
                    if (keyEvent->code == sf::Keyboard::Key::Up)    keyU = false;
                    if (keyEvent->code == sf::Keyboard::Key::Down)  keyD = false;
                }
            }
        }

        return true;
    }

    bool RendererBasic::draw() {
        if(!window->isOpen()) { return false; }

        ImGui::SFML::Update(*(window.get()), deltaClock.restart());

        float moveAmount = 10.0 * currentZoom;
        if(keyL) move(-moveAmount, 0);
        if(keyR) move( moveAmount, 0);
        if(keyU) move(0, -moveAmount);
        if(keyD) move(0,  moveAmount);
    
        window->clear(sf::Color::Black);

        window->setView(gameView);
        renderGame();

        window->setView(guiView);
        ImGui::SetNextWindowBgAlpha(0.0f);
        ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize, ImGuiCond_Always);
        ImGui::Begin("GameOverlay", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove);
        renderGui();
        ImGui::End();
        ImGui::SFML::Render(*(window.get()));

        ColorData cd(sf::Color::Magenta, sf::Color::Red, 1);
        drawRect(520, 220, 20, 20, cd);

        window->display();
        return true;
    }

    void RendererBasic::zoom(float factor) {
        sf::Vector2f mouseWorldBefore = window->mapPixelToCoords(sf::Mouse::getPosition(*window.get()), gameView);
        gameView.zoom(factor);
        sf::Vector2f mouseWorldAfter = window->mapPixelToCoords(sf::Mouse::getPosition(*window.get()), gameView);
        sf::Vector2f movement = mouseWorldBefore - mouseWorldAfter;
        gameView.move(movement);
        currentZoom *= factor;
    }

    void RendererBasic::move(float offsetX, float offsetY) {
        sf::Vector2f offset(offsetX, offsetY);
        gameView.move(offset);
    }

    void RendererBasic::subscribeToEvents() {
        //
    }

    void RendererBasic::renderGame() {
        if(!game) return;
        renderCollisionGrid();
        renderMaps(game->getEntityManager());
        renderEntitiesWithCollision(game->getEntityManager());
    }

    void RendererBasic::renderCollisionGrid() {
        int tileSize = game->getCollisionSystem()->getTileSize();
        int tx = width  / tileSize + 1;
        int ty = height / tileSize + 1;

        sf::Color gray(60, 60, 60);
        ColorData cd(sf::Color::Black, gray, 1);

        for(int i = 0; i < tx; i++) {
            for(int j = 0; j < ty; j++) {
                drawRect(i * tileSize, j * tileSize, tileSize, tileSize, cd);
            }
        }
    }

    void RendererBasic::renderMaps(EntityManager* entityManager) {
        auto maps = entityManager->getEntitiesWithComponents<MapComponent, TilesComponent>();
        for(auto& map : maps) {
            renderMap(map);
        }
    }

    void RendererBasic::renderMap(_EntMapTiles map) {
        ColorData cd_empty     (sf::Color::Black  , sf::Color::White, 2);
        ColorData cd_occupied  (sf::Color::Red    , sf::Color::White, 2);
        ColorData cd_impassable(sf::Color::Blue   , sf::Color::White, 2);
        ColorData cd_obstructed(sf::Color::Magenta, sf::Color::White, 2);

        auto tilesComponent = std::get<2>(map);
        if(!tilesComponent) return;

        int s = tilesComponent->tileSize;
        int x = -1, y = 0, index = -1;
        // for(auto& t : tilesComponent->tiles) {
        //     index++;
        //     if(++x >= tilesComponent->tilesX) {
        //         x = 0; y++;
        //     }

        //     ColorData c;
        //     if(!t->isOccupied && !t->isImpassable) c = cd_empty;
        //     if( t->isOccupied && !t->isImpassable) c = cd_occupied;
        //     if(!t->isOccupied &&  t->isImpassable) c = cd_impassable;
        //     if( t->isOccupied &&  t->isImpassable) c = cd_obstructed;
        //     drawRect(x * s, y * s, s, s, c);
        // }
    }

    void RendererBasic::renderEntitiesWithCollision(EntityManager* entityManager) {
        assert(entityManager);
        auto entities = entityManager->getEntitiesWithComponents<PositionComponent, CollisionComponent>();

        ColorData cd_shape(sf::Color::Green , sf::Color::Blue , 2);
        ColorData cd_dummy(sf::Color::Red   , sf::Color::Blue , 1);
        ColorData cd_bb   (sf::Color::Yellow, sf::Color::Black, 1);
    
        for (auto [entity, pos, col] : entities) {
            // auto* pos = entity->getComponent<PositionComponent>();
            // auto* collision = entity->getComponent<CollisionComponent>();
            if (!pos || !col) continue;
    
            float entityX = pos->x;
            float entityY = pos->y;
            auto& bb = col->boundingBox;

            //drawRect(entityX + bb.x, entityY + bb.y, bb.w, bb.h, cd_bb);
        }

        for (auto [entity, pos, col] : entities) {
            // auto* pos = entity->getComponent<PositionComponent>();
            // auto* collision = entity->getComponent<CollisionComponent>();
            if (!pos || !col) continue;
    
            float entityX = pos->x;
            float entityY = pos->y;
    
            if (!col->shapes.empty()) {
                for (const auto& shape : col->shapes) {
                    float absX = entityX + shape->offsetX;
                    float absY = entityY + shape->offsetY;
    
                    if (auto* circle = dynamic_cast<CircleShape*>(shape.get())) {
                        drawCircle(absX, absY, circle->radius, cd_shape);
                    } 
                    else if (auto* rect = dynamic_cast<RectangleShape*>(shape.get())) {
                        drawRect(absX, absY, rect->width, rect->height, cd_shape);
                    }
                }
            } else {
                if (col->dummyRadius > 0.0f) {
                    drawCircle(entityX, entityY, col->dummyRadius, cd_dummy);
                }
            }
        }

    }

    void RendererBasic::renderGui() {
        if(!gui) return;
        for (const auto element : gui->getGuiElements()) {
            switch (element->type) {
                case GuiElementType::Panel:
                    drawPanel(element->style, element->x, element->y, element->width, element->height);
                    break;
                case GuiElementType::Label:
                    drawLabel(element->style, element->x, element->y, element->text);
                    break;
                case GuiElementType::Button:
                    drawButton(element->style, element->x, element->y, element->width, element->height, element->text);
                    break;
            }
        }
    }

    void RendererBasic::drawPanel(GuiElementStyle style, float x, float y, float width, float height) {
        ColorData cd(sf::Color::Black, sf::Color::Black, 0.0);
        int innerFrameDistance = 1;
        float frameThickness = 1.0;

        switch (style) {
            case GuiElementStyle::Default:
                cd.fillColor = sf::Color::Blue;
                cd.outlineColor = sf::Color::White;
                innerFrameDistance = 0;
                frameThickness = 1.0;
                break;
            case GuiElementStyle::Test:
                cd.fillColor = sf::Color::Magenta;
                cd.outlineColor = sf::Color::Blue;
                innerFrameDistance = 3;
                frameThickness = 2.0;
                break;
            default:
                break;
        }

        if (innerFrameDistance == 0) {
            cd.outlineThickness = frameThickness;
            drawRect(x, y, width, height, cd);
        } else {
            ColorData outer(cd.fillColor, cd.fillColor, 1.0);
            ColorData inner(cd.fillColor, cd.outlineColor, frameThickness);
            drawRect(x, y, width, height, outer);
            drawRect(x + frameThickness, y + frameThickness,
                width - 2 * frameThickness, height - 2 * frameThickness, inner);
        }
        
        // ImGui::SetCursorPos(ImVec2(x, y));
        // ImGui::BeginChild("##panel", ImVec2(width, height), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
        // ImGui::EndChild();
    }   

    void RendererBasic::drawLabel(GuiElementStyle style, float x, float y, const std::string text) {
        ImGui::SetCursorPos(ImVec2(x, y));
        ImGui::Text("%s", text.c_str());
    }   

    void RendererBasic::drawButton(GuiElementStyle style, float x, float y, float width, float height, std::string text) {
        ImGui::SetCursorPos(ImVec2(x, y));
        if (ImGui::Button(text.c_str(), ImVec2(width, height))) {
            std::cout << "Button '" << text << "' clicked!\n";
        }
    }

    void RendererBasic::drawRect(float x, float y, float width, float height, ColorData cData) {
        sf::RectangleShape rectangle(sf::Vector2f(width, height));
        rectangle.setFillColor(cData.fillColor);
        rectangle.setOutlineColor(cData.outlineColor);
        rectangle.setOutlineThickness(cData.outlineThickness);
        rectangle.setPosition(sf::Vector2f(x, y));
        window->draw(rectangle);
    }
    
    void RendererBasic::drawCircle(float x, float y, float r, ColorData cData) {
        sf::CircleShape circle(r);
        circle.setPointCount(20);
        circle.setFillColor(cData.fillColor);
        circle.setOutlineColor(cData.outlineColor);
        circle.setOutlineThickness(cData.outlineThickness);
        circle.setPosition(sf::Vector2f(x - r, y - r));
        window->draw(circle);
    }

    void RendererBasic::drawLine(float x1, float y1, float x2, float y2, sf::Color color) {
        // sf::Vertex line[] = { sf::Vertex(sf::Vector2f(x1, y1)), sf::Vertex(sf::Vector2f(x2, y2)) };
        // line[0].color = color;
        // line[1].color = color;

        //window->draw(line, 2, sf::Lines);
    }


}
    