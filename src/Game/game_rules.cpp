#include <Game/game.hpp>

#pragma once


namespace SupDef {

    Rules* Game::getRules(Entity* entity) {
        while (entity) {
            if (auto rulesComp = entity->getComponent<RulesComponent>()) {
                auto val = rulesComp->getRules();
                if (val) return val;
            }
            entity = entityManager->getParent(entity->id);
        }
        return rules.get();
    }

    Rules* Game::getRules(EntityID entityID) {
        return getRules(entityManager->getEntity(entityID));
    }

    Asset* Game::getAsset(AssetID id) {
        assert(rules);
        return rules->getAsset(id);
    }

    Asset* Game::getAsset(Entity* entity, AssetID id) {
        auto ptr = getRules(entity);
        if (ptr) ptr->getAsset(id);
        return nullptr;
    }

    Asset* Game::getAsset(EntityID entityID, AssetID assetID) {
        return getAsset(entityManager->getEntity(entityID), assetID);
    }

    // template <typename T>
    // std::optional<T> Game::getRuleValue(const std::string& key, Entity* entity) const {
    //     while (entity) {
    //         if (auto rulesComp = entity->getComponent<RulesComponent>()) {
    //             auto val = rulesComp->getRule<T>(key);
    //             if(val) return val;
    //         }
    //         entity = entityManager->getParent(entity->id);
    //     }
    //     return rules->get<T>(key);
    // }

    // template <typename T>
    // std::optional<T> Game::getRuleValue(const std::string& key, EntityID entityID) const {
    //     return getRuleValue<T>(key, entityManager->getEntity(entityID));
    // }

    // template <typename T>
    // std::optional<T> Game::getRuleValue(const std::string& key) const {
    //     return getRuleValue<T>(key, int(NO_ENTITY));
    // }

}
