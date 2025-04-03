#include <Renderer/renderer_basic.hpp>

#pragma once


namespace SupDef {

    void RendererBasic::renderGame() {
        if(!game) return;
        renderMaps(game->getEntityManager());
        renderEntitiesWithCollision(game->getEntityManager());
        renderSelectedUnits();
        renderVirtualEntity();
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
        sf::Color colGray = sf::Color(120, 120, 12,  80);
        ColorData cd_empty     (sf::Color(  0,   0,   0,  80), colGray, 1);
        ColorData cd_occupied  (sf::Color(150,   0,   0,  80), colGray, 1);
        ColorData cd_impassable(sf::Color( 90,  90,  90,  80), colGray, 1);
        ColorData cd_obstructed(sf::Color( 50,   0,   0, 140), colGray, 1);

        auto tilesComponent = std::get<2>(map);
        if (!tilesComponent) return;
        if (!drawTiles) return;

        int s = tilesComponent->tileSize;
        int x = -1, y = 0, index = -1;
        for(auto& t : tilesComponent->tiles) {
            index++;
            if(++x >= tilesComponent->tilesX) {
                x = 0; y++;
            }
            ColorData c;
            if(!t->isOccupied && !t->isImpassable) c = cd_empty;
            if( t->isOccupied && !t->isImpassable) c = cd_occupied;
            if(!t->isOccupied &&  t->isImpassable) c = cd_impassable;
            if( t->isOccupied &&  t->isImpassable) c = cd_obstructed;
            drawRect(x * s, y * s, s, s, c);
        }

        for(auto [px, py] : game->pathTiles) {
            drawRect(px - s/2, py - s/2, s, s, cd_impassable);
        }

        if (debugMode) {
            auto stepFactor = game->getPathFinder()->stepFactor;
            int width  = tilesComponent->tilesX * stepFactor;
            int height = tilesComponent->tilesY * stepFactor;
            int subStep = s / (float) stepFactor;
            for (auto& [key, pn] : game->getPathFinder()->visited) {
                x = key % width;
                y = key / width;
                drawRect(x * subStep, y * subStep, s, s, cd_obstructed);
            }
        }
    }

    void RendererBasic::renderEntitiesWithCollision(EntityManager* entityManager) {
        assert(entityManager);
        auto entities = entityManager->getEntitiesWithComponents<PositionComponent, CollisionComponent>();
        for (auto [entity, pos, col] : entities) {
            if (entity->hasComponent<InfluenceComponent>()) {
                // renderEntityWithInfluence(pos, col);
            } else {
                renderEntityWithCollision(pos, col, false);
            }
        }
    }

    void RendererBasic::renderEntityWithCollision(PositionComponent* pos, CollisionComponent* col, bool drawBB) {
        if (!pos || !col) return;
    
        float entityX = pos->x;
        float entityY = pos->y;

        if (drawBB) {
            ColorData cd_bb(sf::Color::Yellow, sf::Color::Black, 0);
            auto& bb = col->boundingBox;
            drawRect(entityX + bb.x, entityY + bb.y, bb.w, bb.h, cd_bb);
        }

        if (!col->shapes.empty()) {
            ColorData cd_shape(sf::Color::Green, sf::Color::Blue, 0);
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
            ColorData cd_dummy(sf::Color::Red, sf::Color::Blue, 0);
            if (col->dummyRadius > 0.0f) {
                drawCircle(entityX, entityY, col->dummyRadius, cd_dummy);
            }
        }
    }

    void RendererBasic::renderEntityWithInfluence(PositionComponent* pos, CollisionComponent* col) {
        if (!pos || !col) return;
    
        float entityX = pos->x;
        float entityY = pos->y;

        ColorData cd(sf::Color(250, 0, 0, 40), sf::Color(250, 0, 0, 90), 0);
        if (!col->shapes.empty()) {
            for (const auto& shape : col->shapes) {
                float absX = entityX + shape->offsetX;
                float absY = entityY + shape->offsetY;

                if (auto* circle = dynamic_cast<CircleShape*>(shape.get())) {
                    drawCircle(absX, absY, circle->radius, cd);
                } 
                else if (auto* rect = dynamic_cast<RectangleShape*>(shape.get())) {
                    drawRect(absX, absY, rect->width, rect->height, cd);
                }
            }
        } else {
            if (col->dummyRadius > 0.0f) {
                drawCircle(entityX, entityY, col->dummyRadius, cd);
            }
        }
    }

    void RendererBasic::renderSelectedUnits() {
        auto sm = gui->getSelectionManager();
        assert(sm);
        auto list = sm->getSelectedUnits();
        for (auto id : list) {
            renderSelectedUnit(id);
        }
    }

    void RendererBasic::renderSelectedUnit(EntityID entityID) {
        auto entity = game->getEntityManager()->getEntity(entityID);
        if (!entity) return;
        auto posComp = entity->getComponent<PositionComponent>();
        auto colComp = entity->getComponent<CollisionComponent>();
        if (!posComp) return;
        if (!colComp) return;

        auto infs = game->getEntityManager()->getEntitiesWithComponents<PositionComponent, CollisionComponent, InfluenceComponent>(entityID);
        for(auto [entity, pos, col, inf] : infs) {
            renderEntityWithInfluence(pos, col);
        }

        int d = 5;
        ColorData cd(sf::Color::Black, sf::Color::White, 1);
        drawSelection(
            posComp->x - d + colComp->boundingBox.x,
            posComp->y - d + colComp->boundingBox.y,
            colComp->boundingBox.w + 2 * d,
            colComp->boundingBox.h + 2 * d,
            cd
        );
    }

    void RendererBasic::renderVirtualEntity() {
        if (!virtualEntity) return;
        if (commandMode != RCommandMode::BUILD) return;

        auto pos = virtualEntity->getComponent<PositionComponent>();
        auto col = virtualEntity->getComponent<CollisionComponent>();
        if (!pos || !col) return;

        auto& bb = col->boundingBox;
        auto position = getMousePosWorld();
        pos->x = position.x - bb.w / 2;
        pos->y = position.y - bb.h / 2;
        renderEntityWithCollision(pos, col, true);
    }

}
