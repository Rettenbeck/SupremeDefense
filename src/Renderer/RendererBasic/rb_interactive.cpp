#include <Renderer/renderer_basic.hpp>

#pragma once


namespace SupDef {

    void RendererBasic::doScrolling() {
        float moveAmountCur = moveAmount * currentZoom;
        if(keyL) move(-moveAmountCur, 0);
        if(keyR) move( moveAmountCur, 0);
        if(keyU) move(0, -moveAmountCur);
        if(keyD) move(0,  moveAmountCur);
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

    void RendererBasic::onMouseClick(MouseClick button) {
        json j;
        auto mousePos = getMousePos();
        auto element = gui->getGuiInSpot(mousePos.x, mousePos.y);
        if (element) {
            gui->handleClickOnGui(element, button);
            return;
        }

        switch (commandMode) {
            case RCommandMode::MOVE:
                if (button == MLEFT) {
                    if (addPositionData(j)) {
                        auto c = game->getCommandTracker(); assert(c);
                        globalDispatcher->dispatch<TriggerCommandEvent>(c->entityID, c->techID, j);
                    }
                } else if (button == MRIGHT) {
                    globalDispatcher->dispatch<TriggerCommandEvent>();
                }
                break;
            case RCommandMode::BUILD:
                if (button == MLEFT) {
                    if (addVirtualEntityData(j)) {
                        auto c = game->getCommandTracker(); assert(c);
                        globalDispatcher->dispatch<TriggerCommandEvent>(c->entityID, c->techID, j);
                    }
                } else if (button == MRIGHT) {
                    globalDispatcher->dispatch<TriggerCommandEvent>();
                }
                break;
            case RCommandMode::NONE:
                if (button == MLEFT) {
                    auto pos = getMousePosWorld();
                    auto [entity, mapID] = getUnitInSpot(pos.x, pos.y);
                    int id = NO_ENTITY;
                    if (entity) id = entity->id;
                    std::cout << "Entity: " << id << "; map: " << mapID << "\n";
                    currentMap = mapID;
                    globalDispatcher->dispatch<UnitSelectedEvent>(entity);
                } else if (button == MRIGHT) {
                    if (addPositionData(j)) {
                        gui->handleClickMove(j);
                    }
                }
                break;
            default:
                break;
        }
    }

    EntityID RendererBasic::mouseToMap() {
        auto maps = game->getEntityManager()->getEntitiesWithComponents<MapComponent>();
        for (auto& [entity, mapComp] : maps) {
            return entity->id;
        }
        return NO_ENTITY;
    }

    sf::Vector2i RendererBasic::getMousePos() {
        return sf::Mouse::getPosition(*window.get());
    }

    sf::Vector2f RendererBasic::getMousePosWorld() {
        return window->mapPixelToCoords(getMousePos(), gameView);
    }

    void RendererBasic::prepareSelectableList() {
        selectables.clear();
        auto list = game->getEntityManager()
            ->getEntitiesWithComponents<PositionComponent, CollisionComponent, SelectableComponent>();

        for (auto [entity, posComp, colComp, seComp] : list) {
            auto parent = game->getEntityManager()->getParent(entity->id);
            while(true) {
                if (!parent) break;
                auto mapComp = parent->getComponent<MapComponent>();
                if (mapComp) break;
                parent = game->getEntityManager()->getParent(parent->id);
            }
            if (!parent) continue;

            float x1, y1, x2, y2, xm, ym;
            auto bb = &(colComp->boundingBox);
            x1 = posComp->xAbs + bb->x;
            y1 = posComp->yAbs + bb->y;
            x2 = x1 + bb->w;
            y2 = y1 + bb->h;
            
            xm = (x1 + x2) / 2;
            ym = (y1 + y2) / 2;
            selectables.emplace_back(entity, parent, bb, xm, ym);
        }
    }

    std::tuple<Entity*, EntityID> RendererBasic::getUnitInSpot(float worldX, float worldY) {
        Entity* sEntity = nullptr;
        EntityID mapID = NO_ENTITY;
        float distance = 999999;

        for (auto [entity, map, bb, xm, ym] : selectables) {
            float x1 = xm - bb->w / 2;
            float y1 = ym - bb->h / 2;
            float x2 = x1 + bb->w;
            float y2 = y1 + bb->h;
            if (worldX >= x1 && worldX <= x2 && worldY >= y1 && worldY <= y2) {
                float tmp = Math::getDistance(worldX, worldY, xm, ym);
                if (distance > tmp) {
                    sEntity = entity;
                    mapID = map->id;
                    distance = tmp;
                }
            }
        }

        if (!sEntity) {
            auto mapID = getMapByCoords(worldX, worldY);
            return {nullptr, mapID};
        }
        return {sEntity, mapID};
    }

    EntityID RendererBasic::getMapByCoords(float worldX, float worldY) {
        auto maps = game->getEntityManager()->getEntitiesWithComponents<MapComponent, PositionComponent>();
        for(auto& [map, mapComp, posComp] : maps) {
            float x1 = posComp->x;
            float y1 = posComp->y;
            float x2 = x1 + mapComp->width;
            float y2 = y1 + mapComp->height;

            if (worldX >= x1 && worldX <= x2 && worldY >= y1 && worldY <= y2) {
                return map->id;
            }
        }
        return NO_ENTITY;
    }

}
