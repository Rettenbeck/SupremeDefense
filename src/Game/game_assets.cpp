#ifndef GAME_ASSETS
#define GAME_ASSETS

#include <Game/game.hpp>


namespace SupDef {

    Entity* Game::createEntityFromAsset(Entity* asset) {
        auto entity = entityManager->createEntity(asset->id);
        auto id = entity->id;
        json j;
        asset->to_json_skip_assets(j);
        entity->from_json(j);
        entity->id = id;

        auto initComp = asset->getComponent<InitContainerComponent>();
        if (initComp) {
            for (auto& containedID : initComp->contained) {
                createEntityFromAsset(containedID, id);
            }
        }
        return entity;
    }

    Entity* Game::createEntityFromAsset(AssetID assetID) {
        auto asset = assetManager->getAsset(assetID);
        assert(asset);
        return createEntityFromAsset(asset);
    }

    Entity* Game::createEntityFromAsset(AssetID assetID, EntityID parentID) {
        auto entity = createEntityFromAsset(assetID);
        setParentOfNewEntity(assetID, entity, parentID);
        return entity;
    }

    Entity* Game::createEntityFromAsset(AssetID assetID, EntityID parentID, EntityID ownerID) {
        auto entity = createEntityFromAsset(assetID);
        setParentOfNewEntity(assetID, entity, parentID);
        setInitialOwner(entity, ownerID);
        return entity;
    }

    Entity* Game::createEntityFromAsset(AssetID assetID, float x, float y) {
        auto entity = createEntityFromAsset(assetID);
        auto posComp = entity->getComponent<PositionComponent>();
        assert(posComp);
        setNewCenteredPosition(entity, posComp, x, y);
        return entity;
    }

    Entity* Game::createEntityFromAsset(AssetID assetID, EntityID parentID, float x, float y) {
        auto entity = createEntityFromAsset(assetID, x, y);
        setParentOfNewEntity(assetID, entity, parentID);
        return entity;
    }

    Entity* Game::createEntityFromAsset(AssetID assetID, EntityID parentID, EntityID ownerID, float x, float y) {
        auto entity = createEntityFromAsset(assetID, parentID, x, y);
        setInitialOwner(entity, ownerID);
        return entity;
    }

    void Game::setParentOfNewEntity(AssetID assetID, Entity* entity, EntityID parentID) {
        assert(entity);
        auto asset = assetManager->getAsset(assetID);
        assert(asset);
        entityManager->setParent(entity->id, parentID);
        auto initPosComp = asset->getComponent<InitPositionComponent>();
        if (initPosComp) {
            setInitialPosition(entity, initPosComp);
        }
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
    
    void Game::setInitialPosition(Entity* entity, InitPositionComponent* initComp) {
        assert(entity);
        assert(initComp);
        
        if (initComp->relativeToMap) {
            auto mapID = getMapOfEntity(entity->id);
            if (mapID != NO_ENTITY) {
                auto map = entityManager->getEntity(mapID);
                assert(map);
                auto posComp = map->getComponent<PositionComponent>();
                assert(posComp);
                float x = initComp->x + posComp->x;
                float y = initComp->y + posComp->y;
                setNewCenteredPosition(entity, x, y);
                return;
            }
        }
        setNewCenteredPosition(entity, initComp->x, initComp->y);
    }

}

#endif
