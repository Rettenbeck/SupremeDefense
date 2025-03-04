#include <Game/game.hpp>

#pragma once


namespace SupDef {

    void Game::processActions() {
        assert(actionQueue);
        for (auto& action : actionQueue->getActions()) {
            assert(action);
            auto asset = getAssetFromCommand(action->commandID, action->data);
            assert(asset);
            processAction(asset, action->data);
        }
    }

    void Game::processAction(Entity* asset, json &data) {
        auto reqComp = asset->getComponent<RequirementComponent>();
        if (reqComp) {
            auto result = checkRequirements(reqComp, CommandStatus::CONFIRMED, true);
        }

        auto buildComp = asset->getComponent<BuildCommandComponent>();
        if (buildComp) {
            assert(virtualEntity);
            auto posComp = virtualEntity->getComponent<PositionComponent>();
            assert(posComp);
            float x, y;
            assert(data.contains("x"));
            assert(data.contains("y"));
            x = data["x"];
            y = data["y"];
            posComp->x = x;
            posComp->y = y;
            realizeVirtualEntity();
        }
    }

}
