#include <Game/game.hpp>

#pragma once


namespace SupDef {

    Entity* Game::createEntityFromAsset(Entity* asset) {
        auto entity = entityManager->createEntity();
        json j;
        asset->to_json_skip_assets(j);
        entity->from_json(j);
        return entity;
    }

    Entity* Game::createEntityFromAsset(AssetID assetID) {
        auto asset = assetManager->getAsset(assetID);
        assert(asset);
        return createEntityFromAsset(asset);
    }

    Entity* Game::createEntityFromAsset(AssetID assetID, EntityID parentID) {
        auto entity = createEntityFromAsset(assetID);
        entityManager->setParent(entity->id, parentID);
        return entity;
    }

    Entity* Game::createEntityFromAsset(AssetID assetID, float x, float y) {
        auto entity = createEntityFromAsset(assetID);
        auto posComp = entity->getComponent<PositionComponent>();
        assert(posComp);
        posComp->x = x; posComp->y = y;
        return entity;
    }

    Entity* Game::createEntityFromAsset(AssetID assetID, EntityID parentID, float x, float y) {
        auto entity = createEntityFromAsset(assetID, x, y);
        entityManager->setParent(entity->id, parentID);
        return entity;
    }

    // Rules* Game::getRules(Entity* entity) {
    //     while (entity) {
    //         if (auto rulesComp = entity->getComponent<RulesComponent>()) {
    //             auto val = rulesComp->getRules();
    //             if (val) return val;
    //         }
    //         entity = entityManager->getParent(entity->id);
    //     }
    //     return rules.get();
    // }

    // Rules* Game::getRules(EntityID entityID) {
    //     return getRules(entityManager->getEntity(entityID));
    // }

    // Asset* Game::getAsset(AssetID id) {
    //     assert(rules);
    //     return rules->getAsset(id);
    // }

    // Asset* Game::getAsset(Entity* entity, AssetID id) {
    //     auto ptr = getRules(entity);
    //     if (ptr) ptr->getAsset(id);
    //     return nullptr;
    // }

    // Asset* Game::getAsset(EntityID entityID, AssetID assetID) {
    //     return getAsset(entityManager->getEntity(entityID), assetID);
    // }

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
