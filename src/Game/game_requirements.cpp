#ifndef GAME_REQ
#define GAME_REQ

#include <Game/game.hpp>


namespace SupDef {
    
    bool Game::checkRequirements(CommandID commandID, json &data, bool onAction) {
        assert(assetManager);
        auto asset = getAssetFromCommand(commandID, data);
        assert(asset);
        auto reqComp = asset->getComponent<RequirementComponent>();
        if (!reqComp) return (thisPlayer != NO_ENTITY);
        return checkRequirements(reqComp, data, onAction);
    }

    bool Game::checkRequirements(RequirementComponent* reqComp, json &data, bool onAction) {
        auto player = entityManager->getEntity(thisPlayer);
        if (!player) return false;
        auto playerComp = player->getComponent<PlayerComponent>();
        if (!playerComp) return false;
        
        if (!checkResourceReq(player, reqComp, data, onAction)) return false;
        // Additional checks...
        return true;
    }

    bool Game::checkResourceReq(Entity* player, RequirementComponent* reqComp, json &data, bool onAction) {
        assert(player);
        assert(reqComp);
        if (!reqComp->resources.empty()) {
            auto resComp = player->getComponent<ResourceComponent>();
            if (!resComp) return false;
            for(const auto& resourceReq : reqComp->resources) {
                if (!resourceReq) continue;
                auto it = resComp->resources.find(resourceReq->resourceID);
                if (it == resComp->resources.end()) return false;
                auto& resourcePlayer = it->second;
                if (resourcePlayer->amount < resourceReq->amount) return false;
                if (onAction) {
                    resourcePlayer->amount -= resourceReq->amount;
                }
            }
        }
        return true;
    }
    
}

#endif
