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
        // std::cout << "\n--- PLAYERS -----------------------------\n";
        for (auto& [mapEntity, mapComp] : maps) {
            // std::cout << "MapID: " << mapEntity->id << "\n";
            for (auto& [x, y, playerRole, playerID1] : mapComp->playerSpawns) {
                Entity* player = nullptr;
                auto tmpIndex = i++;
                // std::cout << "  tmpIndex: " << tmpIndex << "\n";
                // std::cout << "  x: " << x << "\n";
                // std::cout << "  y: " << y << "\n";
                // std::cout << "  playerRole: " << playerRole << "\n";
                // std::cout << "  playerID1: " << playerID1 << "\n";
                for (auto& [playerIndex, playerAsset, playerID2] : playerMapExt_) {
                    if (tmpIndex != playerIndex) continue;
                    // std::cout << "    playerIndex: " << playerIndex << "\n";
                    // std::cout << "    playerAsset: " << playerAsset << "\n";
                    // std::cout << "    playerID2: " << playerID2 << "\n";
                    player = createEntityFromAsset(playerAsset);
                    assert(player);
                    playerID1 = player->id;
                    playerID2 = player->id;
                    // std::cout << "    > player->id: " << player->id << "\n";
                    if (thisPlayer_ == tmpIndex) {
                        thisPlayer = player->id;
                        // std::cout << "      > thisPlayer: " << thisPlayer << "\n";
                    }
                    break;
                }
                auto playerComp = player->getComponent<PlayerComponent>();
                assert(playerComp);
                playerComp->playerRole = playerRole;
                // std::cout << "  > playerComp->playerRole: " << playerComp->playerRole << "\n";
                if (!playerComp->starterBuilding.empty()) {
                    createEntityFromAsset(playerComp->starterBuilding, mapEntity->id, player->id, x, y);
                    // std::cout << "    > playerComp->starterBuilding: " << playerComp->starterBuilding << "\n";
                }
            }
        }

        // Add teams
        // std::cout << "\n--- TEAMS -----------------------------\n";
        for (auto& [playerIndex1, teamIndex, playerID1, teamID] : worldComp->playerList) {
            // std::cout << "playerIndex1: " << playerIndex1 << "\n";
            // std::cout << "teamIndex: " << teamIndex << "\n";
            teamID = NO_ENTITY;
            if (teamIndex == 0) {
                for (auto& [playerIndex2, playerAsset, playerID2] : playerMapExt_) {
                    if (playerIndex1 != playerIndex2) continue;
                    playerID1 = playerID2;
                    break;
                }
                continue;
            }
            for (auto& [playerIndex2, playerAsset, playerID2] : playerMapExt_) {
                if (playerIndex1 != playerIndex2) continue;
                // std::cout << "  playerIndex2: " << playerIndex2 << "\n";
                // std::cout << "  playerAsset: " << playerAsset << "\n";
                // std::cout << "  playerID2: " << playerID2 << "\n";

                EntityID existingTeamID = NO_ENTITY;
                for (auto& [playerIndex_, teamIndex_, playerID_, teamID_] : worldComp->playerList) {
                    if (teamIndex_ == teamIndex) {
                        existingTeamID = teamID_;
                        break;
                    }
                }

                if (existingTeamID == NO_ENTITY) {
                    auto team = entityManager->createEntity();
                    team->addComponent<TeamComponent>();
                    entityManager->setParent(playerID2, team->id);
                    teamID = team->id;
                } else {
                    teamID = existingTeamID;
                }
                playerID1 = playerID2;
                entityManager->setParent(playerID2, teamID);
                // std::cout << "  > teamID: " << teamID << "\n";
                // std::cout << "  > playerID1: " << playerID1 << "\n";
                // std::cout << "  > playerID2: " << playerID2 << "\n";
                break;
            }
        }

        // Add enemy goals
        i = 0;
        // std::cout << "\n--- GOALS -----------------------------\n";
        for (auto& [mapEntity, mapComp] : maps) {
            // std::cout << "MapID: " << mapEntity->id << "\n";
            for (auto& [x, y] : mapComp->enemyGoals) {
                i++;
                // std::cout << "  i: " << i << "\n";
                for (auto& [goalIndex, playerIndex1, teamIndex1, goalID] : worldComp->enemyGoalList) {
                    if (i != goalIndex) continue;
                    // std::cout << "    goalIndex: " << goalIndex << "\n";
                    // std::cout << "    playerIndex1: " << playerIndex1 << "\n";
                    // std::cout << "    teamIndex1: " << teamIndex1 << "\n";
                    // std::cout << "    goalID: " << goalID << "\n";
                    for (auto& [playerIndex2, teamIndex2, playerID, teamID] : worldComp->playerList) {
                        if (playerIndex1 != 0) if (playerIndex1 != playerIndex2) continue;
                        if (teamIndex1   != 0) if (teamIndex1   != teamIndex2  ) continue;
                        // std::cout << "      playerIndex2: " << playerIndex2 << "\n";
                        // std::cout << "      teamIndex2: " << teamIndex2 << "\n";
                        // std::cout << "      playerID: " << playerID << "\n";
                        // std::cout << "      teamID: " << teamID << "\n";

                        auto goal = entityManager->createEntity(mapEntity->id);
                        goal->addComponent<PositionComponent>();
                        setNewCenteredPosition(goal, x, y);
                        auto goalComp = goal->addComponent<EnemyGoalComponent>();
                        if (playerIndex1 != 0) goalComp->playerID = playerID;
                        if (teamIndex1   != 0) goalComp->teamID   = teamID  ;
                        goalID = goal->id;
                        // std::cout << "    > playerID: " << playerID << "\n";
                        // std::cout << "    > teamID: " << teamID << "\n";
                        // std::cout << "    > goalID: " << goalID << "\n";
                        break;
                    }
                    break;
                }
            }
        }
        
        // Add enemy spawners
        i = 0;
        // std::cout << "\n--- SPAWNERS -----------------------------\n";
        for (auto& [mapEntity, mapComp] : maps) {
            // std::cout << "MapID: " << mapEntity->id << "\n";
            for (auto& [x, y] : mapComp->enemySpawns) {
                i++;
                // std::cout << "  i: " << i << "\n";
                // std::cout << "  x: " << x << "\n";
                // std::cout << "  y: " << y << "\n";
                for (auto& [spawnIndex, wavesID, goalIndex1] : worldComp->enemySpawnList) {
                    if (i != spawnIndex) continue;
                    // std::cout << "    spawnIndex: " << spawnIndex << "\n";
                    // std::cout << "    wavesID: " << wavesID << "\n";
                    // std::cout << "    goalIndex1: " << goalIndex1 << "\n";
                    for (auto& [goalIndex2, playerIndex, teamIndex, goalID] : worldComp->enemyGoalList) {
                        if (goalIndex1 != goalIndex2) continue;
                        // std::cout << "      goalIndex2: " << goalIndex2 << "\n";
                        // std::cout << "      playerIndex: " << playerIndex << "\n";
                        // std::cout << "      teamIndex: " << teamIndex << "\n";
                        // std::cout << "      goalID: " << goalID << "\n";

                        auto spawner = entityManager->createEntity(mapEntity->id);
                        spawner->addComponent<PositionComponent>();
                        setNewCenteredPosition(spawner, x, y);
                        auto spawnerComp = spawner->addComponent<EnemySpawnerComponent>();
                        spawnerComp->goalID = goalID;
                        spawnerComp->wavesID = wavesID;
                        // std::cout << "      > spawnerComp->goalID: " << spawnerComp->goalID << "\n";
                        // std::cout << "      > spawnerComp->wavesID: " << spawnerComp->wavesID << "\n";
                        break;
                    }
                    break;
                }
            }
        }

        serialize("startup.txt");
    }

}

#endif
