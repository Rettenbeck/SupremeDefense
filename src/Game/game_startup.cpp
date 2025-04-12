// #pragma once
#ifndef GAME_STARTUP
#define GAME_STARTUP

#include <Game/game.hpp>


namespace SupDef {

    void Game::startWorld(AssetID worldID, PlayerMapExt playerMapExt_, int thisPlayer_) {
        auto world = createEntityFromAsset(worldID);
        auto worldComp = world->getComponent<WorldComponent>();
        assert(worldComp);
        auto maps = entityManager->getEntitiesWithComponents<MapComponent>();
        
        // Create players and their starter units
        int i = 1;
        for (auto& [mapEntity, mapComp] : maps) {
            for (auto& [x, y, playerRole, playerID1] : mapComp->playerSpawns) {
                Entity* player = nullptr;
                auto tmpIndex = i++;
                for (auto& [playerIndex, playerAsset, playerID2] : playerMapExt_) {
                    if (tmpIndex != playerIndex) continue;
                    player = createEntityFromAsset(playerAsset);
                    assert(player);
                    playerID1 = player->id;
                    playerID2 = player->id;
                    if (thisPlayer_ == tmpIndex) {
                        thisPlayer = player->id;
                    }
                    break;
                }
                auto playerComp = player->getComponent<PlayerComponent>();
                assert(playerComp);
                playerComp->playerRole = playerRole;
                if (!playerComp->starterBuilding.empty()) {
                    createEntityFromAsset(playerComp->starterBuilding, mapEntity->id, player->id, x, y);
                }
            }
        }

        // Add teams
        for (auto& [playerIndex1, teamIndex, playerID1, teamID] : worldComp->playerList) {
            teamID = NO_ENTITY;
            if (teamIndex == 0) continue;
            for (auto& [playerIndex2, playerAsset, playerID2] : playerMapExt_) {
                if (playerIndex1 != playerIndex2) continue;
                playerID1 = playerID2;
                auto team = entityManager->createEntity();
                team->addComponent<TeamComponent>();
                entityManager->setParent(playerID2, team->id);
                teamID = team->id;
                break;
            }
        }

        // Add enemy goals
        i = 0;
        for (auto& [mapEntity, mapComp] : maps) {
            for (auto& [x, y] : mapComp->enemyGoals) {
                i++;
                for (auto& [goalIndex, playerIndex1, teamIndex1, goalID] : worldComp->enemyGoalList) {
                    if (i != goalIndex) continue;
                    for (auto& [playerIndex2, teamIndex2, playerID, teamID] : worldComp->playerList) {
                        if (playerIndex1 != 0) if (playerIndex1 != playerIndex2) continue;
                        if (teamIndex1   != 0) if (teamIndex1   != teamIndex2  ) continue;

                        auto goal = entityManager->createEntity(mapEntity->id);
                        goal->addComponent<PositionComponent>();
                        setNewCenteredPosition(goal, x, y);
                        auto goalComp = goal->addComponent<EnemyGoalComponent>();
                        if (playerIndex1 != 0) goalComp->playerID = playerID;
                        if (teamIndex1   != 0) goalComp->teamID   = teamID  ;
                        goalID = goal->id;
                        break;
                    }
                    break;
                }
            }
        }
        
        // Add enemy spawners
        i = 0;
        for (auto& [mapEntity, mapComp] : maps) {
            for (auto& [x, y] : mapComp->enemyGoals) {
                i++;
                for (auto& [spawnIndex, wavesID, goalIndex1] : worldComp->enemySpawnList) {
                    if (i != spawnIndex) continue;
                    for (auto& [goalIndex2, playerIndex, teamIndex, goalID] : worldComp->enemyGoalList) {
                        if (goalIndex1 != goalIndex2) continue;

                        auto spawner = entityManager->createEntity(mapEntity->id);
                        spawner->addComponent<PositionComponent>();
                        setNewCenteredPosition(spawner, x, y);
                        auto spawnerComp = spawner->addComponent<EnemySpawnerComponent>();
                        spawnerComp->goalID = goalID;
                        spawnerComp->wavesID = wavesID;
                        break;
                    }
                    break;
                }
            }
        }
    }

    // Entity* Game::createPlayer(AssetID playerID, AssetID spawnID, EntityID mapID) {
    //     auto player = createEntityFromAsset(playerID);
    //     assert(player);
    //     auto playerComp = player->getComponentStrict<PlayerComponent>();
    //     assert(playerComp->starterBuilding != NO_ASSET);
    //     auto starter = createEntityFromAsset(playerComp->starterBuilding, mapID, player->id);
    //     auto spawnAsset = assetManager->getAssetStrict(spawnID);
    //     auto initPos = spawnAsset->getComponentStrict<InitPositionComponent>();
    //     setInitialPosition(starter, initPos);
    //     return player;
    // }

    // EntityID Game::getMapIDBySpawn(AssetID spawnAssetID) {
    //     AssetID mapAssetID = NO_ASSET;
    //     auto mapAssets = assetManager->getAssetsWithComponents<MapComponent, InitContainerComponent>();
    //     for (auto [asset, mapComp, initComp] : mapAssets) {
    //         for (auto contained : initComp->contained) {
    //             if (contained == spawnAssetID) {
    //                 mapAssetID = asset->assetID;
    //                 break;
    //             }
    //         }
    //         if (mapAssetID != NO_ASSET) break;
    //     }
    //     if (mapAssetID == NO_ASSET) return NO_ENTITY;

    //     auto mapEntities = entityManager->getEntitiesWithComponents<MapComponent>();
    //     for (auto [entity, mapComp] : mapEntities) {
    //         if (entity->assetID == mapAssetID) return entity->id;
    //     }
    //     return NO_ENTITY;
    // }

}

#endif
