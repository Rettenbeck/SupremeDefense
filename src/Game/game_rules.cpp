#include <Game/game.hpp>

#pragma once


namespace SupDef {

    template <typename T>
    std::optional<T> Game::getRuleValue(const std::string& key, Entity* entity) const {
        while (entity) {
            if (auto rulesComp = entity->getComponent<RulesComponent>()) {
                auto val = rulesComp->getRule<T>(key);
                if(val) return val;
            }
            entity = entityManager->getParent(entity->id);
        }
        return rules->get<T>(key);
    }

    template <typename T>
    std::optional<T> Game::getRuleValue(const std::string& key, EntityID entityID) const {
        return getRuleValue<T>(key, entityManager->getEntity(entityID));
    }

    template <typename T>
    std::optional<T> Game::getRuleValue(const std::string& key) const {
        return getRuleValue<T>(key, int(NO_ENTITY));
    }

}
