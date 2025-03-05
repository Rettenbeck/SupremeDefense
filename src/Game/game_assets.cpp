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

    Entity* Game::createEntityFromAsset(AssetID assetID, EntityID parentID, EntityID ownerID) {
        auto entity = createEntityFromAsset(assetID);
        entityManager->setParent(entity->id, parentID);
        setInitialOwner(entity, ownerID);
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

    Entity* Game::createEntityFromAsset(AssetID assetID, EntityID parentID, EntityID ownerID, float x, float y) {
        auto entity = createEntityFromAsset(assetID, parentID, x, y);
        setInitialOwner(entity, ownerID);
        return entity;
    }

    Entity* Game::realizeVirtualEntity() {
        if (!virtualEntity) return nullptr;
        return entityManager->realizeEntity(std::move(virtualEntity));
    }

    void Game::createVirtualEntityFromAsset(Entity* asset) {
        virtualEntity = std::move(entityManager->createVirtualEntity(asset->assetID));
        json j;
        asset->to_json_skip_assets(j);
        virtualEntity->from_json(j);
    }

    void Game::createVirtualEntityFromAsset(AssetID assetID) {
        auto asset = assetManager->getAsset(assetID);
        assert(asset);
        createVirtualEntityFromAsset(asset);
    }
    
    void Game::setInitialOwner(Entity* entity, EntityID ownerID) {
        if (!entity) return;
        auto ownComp = entity->getComponent<PlayerOwnershipComponent>();
        if (!ownComp) {
            ownComp = entity->addComponent<PlayerOwnershipComponent>();
        }
        ownComp->setInitialOwner(ownerID);
    }

    void Game::setInitialOwner(EntityID entityID, EntityID ownerID) {
        setInitialOwner(entityManager->getEntity(entityID), ownerID);
    }
    
}
