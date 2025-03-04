#include <Renderer/renderer_basic.hpp>

#pragma once


namespace SupDef {

    void RendererBasic::renderGame() {
        if(!game) return;
        renderCollisionGrid();
        renderMaps(game->getEntityManager());
        renderEntitiesWithCollision(game->getEntityManager());

        if (commandMode == RCommandMode::BUILD) {
            ColorData cd(sf::Color::Yellow, sf::Color::White, 2);
            auto pos = getMousePosWorld();
            int w = 20, h = 16;
            drawRect(pos.x - w /2, pos.y - h / 2, w, h, cd);
        }
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

}
