#include <Game/game.hpp>

#pragma once


namespace SupDef {

    void Game::processActions() {
        assert(actionQueue);
        for (auto& action : actionQueue->getActions()) {
            assert(action);
            auto command = getAssetFromCommand(action->commandID, action->data);
            assert(command);
            processAction(command, action->playerID, action->data);
        }
        actionQueue->clear();
    }

    void Game::processAction(Entity* command, EntityID playerID, json &data) {
        auto player = entityManager->getEntity(playerID);
        assert(player);
        auto playerComp = player->getComponent<PlayerComponent>();
        assert(playerComp);

        auto reqComp = command->getComponent<RequirementComponent>();
        if (reqComp) {
            auto result = checkRequirements(reqComp, data, CommandStatus::CONFIRMED, true);
        }

        auto buildComp = command->getComponent<BuildCommandComponent>();
        if (buildComp) {
            assert(data.contains(JCOM_X));
            assert(data.contains(JCOM_Y));
            assert(data.contains(JCOM_PARENT));
            auto x = data[JCOM_X];
            auto y = data[JCOM_Y];
            auto parentID = data[JCOM_PARENT];
            createEntityFromAsset(buildComp->toBuild, parentID, playerID, x, y);
        }
    }

}
