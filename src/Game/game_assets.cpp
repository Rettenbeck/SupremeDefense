#include <Game/game.hpp>

#pragma once


namespace SupDef {

    Entity* Game::createEntityFromAsset(Entity* asset) {
        auto entity = entityManager->createEntity(asset->id);
        auto id = entity->id;
        json j;
        asset->to_json_skip_assets(j);
        entity->from_json(j);
        entity->id = id;
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

}
