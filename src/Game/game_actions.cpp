#include <Game/game.hpp>

#pragma once


namespace SupDef {

    void Game::processActions() {
        assert(actionQueue);
        for (auto& action : actionQueue->getActions()) {
            assert(action);
            auto command = getAssetFromCommand(action->commandID, action->data);
            assert(command);
            processAction(command, action->data);
        }
        actionQueue->clear();
    }

    void Game::processAction(Entity* command, json &data) {
        auto reqComp = command->getComponent<RequirementComponent>();
        if (reqComp) {
            auto result = checkRequirements(reqComp, CommandStatus::CONFIRMED, true);
        }

        auto buildComp = command->getComponent<BuildCommandComponent>();
        if (buildComp) {
            assert(data.contains(JCOM_X));
            assert(data.contains(JCOM_Y));
            assert(data.contains(JCOM_PARENT));
            auto x = data[JCOM_X];
            auto y = data[JCOM_Y];
            auto parentID = data[JCOM_PARENT];
            createEntityFromAsset(buildComp->toBuild, parentID, x, y);
        }
    }

}
