#include <Game/game.hpp>

#pragma once


namespace SupDef {

    void Game::processActions() {
        assert(actionQueue);
        for (auto& action : actionQueue->getActions()) {
            assert(action);
            auto command = getAssetFromCommand(action->commandID, action->data);
            assert(command);
            processAction(command, action->entityID, action->playerID, action->data);
        }
        actionQueue->clear();
    }

    void Game::processAction(Entity* command, EntityID entityID, EntityID playerID, json &data) {
        auto player = entityManager->getEntity(playerID);
        assert(player);
        auto playerComp = player->getComponent<PlayerComponent>();
        assert(playerComp);

        auto reqComp = command->getComponent<RequirementComponent>();
        if (reqComp) {
            auto result = checkRequirements(reqComp, data, true);
        }

        auto buildComp = command->getComponent<BuildCommandComponent>();
        if (buildComp) {
            assert(data.contains(JCOM_X));
            assert(data.contains(JCOM_Y));
            assert(data.contains(JCOM_MAP));
            auto x = data[JCOM_X];
            auto y = data[JCOM_Y];
            auto mapID = data[JCOM_MAP];
            createEntityFromAsset(buildComp->toBuild, mapID, playerID, x, y);
        }

        auto moveComp = command->getComponent<MoveCommandComponent>();
        if (moveComp) {
            assert(data.contains(JCOM_X));
            assert(data.contains(JCOM_Y));
            assert(data.contains(JCOM_MAP));
            auto x = data[JCOM_X];
            auto y = data[JCOM_Y];
            auto mapID = data[JCOM_MAP];
            auto map = entityManager->getEntity(mapID);
            auto entity = entityManager->getEntity(entityID);
            assert(entity);
            assert(map);
            auto mapComp = map->getComponent<MapComponent>();
            auto tilesComp = map->getComponent<TilesComponent>();
            auto mapPosComp = map->getComponent<PositionComponent>();
            assert(mapComp);
            assert(tilesComp);
            assert(mapPosComp);
            float mx1 = mapPosComp->x, my1 = mapPosComp->y;
            float mx2 = mx1 + mapComp->width, my2 = my1 + mapComp->height;
            if (x > mx1 && y > my1 && x < mx2 && y < my2) {
                auto posComp = entity->getComponent<PositionComponent>();
                auto movComp = entity->getComponent<MovementComponent>();
                assert(posComp);
                assert(movComp);
                movComp->setGoal(x, y, posComp->xAbs, posComp->yAbs);
                updateTempGoal(tilesComp, entity);
                // std::cout << "Goal set! " << x << "; " << y << "\n"; 
            } else {
                // std::cout << "invalid; x: " << x << "; y: " << y << "\n";
                // std::cout << "  mx1: " << mx1 << "; my1: " << my1 << "; mx2: " << mx2 << "; my2: " << my2 << "\n";
            }
        }
    }

}
