#pragma once

#include <Game/game.hpp>


namespace SupDef {

    void Game::startWorld(AssetID worldID, PlayerMapExt playerMapExt_, int thisPlayer_) {
        auto world = createEntityFromAsset(worldID);
        for (auto [extID, playerAssetID, spawnAssetID, playerEntityID] : playerMapExt_) {
            auto mapID = getMapIDBySpawn(spawnAssetID);
            assert(mapID != NO_ENTITY);
            auto newPlayer = createPlayer(playerAssetID, spawnAssetID, mapID);
            playerEntityID = newPlayer->id;
            if (extID = thisPlayer_) thisPlayer = newPlayer->id;
        }
        playerMapExt = playerMapExt_;
    }

    Entity* Game::createPlayer(AssetID playerID, AssetID spawnID, EntityID mapID) {
        auto player = createEntityFromAsset(playerID);
        assert(player);
        auto playerComp = player->getComponentStrict<PlayerComponent>();
        assert(playerComp->starterBuilding != NO_ASSET);
        auto starter = createEntityFromAsset(playerComp->starterBuilding, mapID, player->id);
        auto spawnAsset = assetManager->getAssetStrict(spawnID);
        auto initPos = spawnAsset->getComponentStrict<InitPositionComponent>();
        setInitialPosition(starter, initPos);
        return player;
    }

    EntityID Game::getMapIDBySpawn(AssetID spawnAssetID) {
        AssetID mapAssetID = NO_ASSET;
        auto mapAssets = assetManager->getAssetsWithComponents<MapComponent, InitContainerComponent>();
        for (auto [asset, mapComp, initComp] : mapAssets) {
            for (auto contained : initComp->contained) {
                if (contained == spawnAssetID) {
                    mapAssetID = asset->assetID;
                    break;
                }
            }
            if (mapAssetID != NO_ASSET) break;
        }
        if (mapAssetID == NO_ASSET) return NO_ENTITY;

        auto mapEntities = entityManager->getEntitiesWithComponents<MapComponent>();
        for (auto [entity, mapComp] : mapEntities) {
            if (entity->assetID == mapAssetID) return entity->id;
        }
        return NO_ENTITY;
    }

}
