#ifndef GAME_STARTUP
#define GAME_STARTUP

#include <Game/game.hpp>


namespace SupDef {

    void Game::startWorld(UInitialConditions initialConditions) {
        assert(initialConditions);
        startWorld(initialConditions->worldID, initialConditions->playerMapExt, initialConditions->thisPlayer);
    }

    void Game::startWorld(AssetID worldID, PlayerMapExt playerMapExt_, int thisPlayer_) {
        auto world = createEntityFromAsset(worldID);
        auto worldComp = world->getComponent<WorldComponent>();
        assert(worldComp);
        auto maps = entityManager->getEntitiesWithComponents<MapComponent>();
        
        // Create enemy player
        auto enemyPlayer_ = entityManager->createEntity();
        enemyPlayer = enemyPlayer_->id;
        enemyPlayer_->addComponent<PlayerComponent>();

        // Create players and their starter units
        int i = 1;
        for (auto& [mapEntity, mapComp] : maps) {
            for (auto& [x, y, playerRole, playerId] : mapComp->playerSpawns) {
                Entity* player = nullptr;
                auto tmpIndex = i++;
                for (auto& playerMapExtSingle : playerMapExt_) {
                    if (tmpIndex != playerMapExtSingle.playerIndex) continue;
                    player = createEntityFromAsset(playerMapExtSingle.playerAsset);
                    assert(player);
                    playerId = player->id;
                    playerMapExtSingle.playerEntityId = player->id;
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
        for (auto& [playerIndex, teamIndex, playerId, teamId] : worldComp->playerList) {
            teamId = NO_ENTITY;
            if (teamIndex == 0) {
                for (auto& playerMapExtSingle : playerMapExt_) {
                    if (playerIndex != playerMapExtSingle.playerIndex) continue;
                    playerId = playerMapExtSingle.playerEntityId;
                    break;
                }
                continue;
            }
            for (auto& playerMapExtSingle : playerMapExt_) {
                if (playerIndex != playerMapExtSingle.playerIndex) continue;

                EntityID existingTeamId = NO_ENTITY;
                for (auto& [playerIndex_, teamIndex_, playerId_, teamId_] : worldComp->playerList) {
                    if (teamIndex_ == teamIndex) {
                        existingTeamId = teamId_;
                        break;
                    }
                }

                if (existingTeamId == NO_ENTITY) {
                    auto team = entityManager->createEntity();
                    team->addComponent<TeamComponent>();
                    entityManager->setParent(playerMapExtSingle.playerEntityId, team->id);
                    teamId = team->id;
                } else {
                    teamId = existingTeamId;
                }
                playerId = playerMapExtSingle.playerEntityId;
                entityManager->setParent(playerId, teamId);
                break;
            }
        }

        // Add enemy goals
        i = 0;
        for (auto& [mapEntity, mapComp] : maps) {
            for (auto& [x, y] : mapComp->enemyGoals) {
                i++;
                for (auto& [goalIndex, playerIndex1, teamIndex1, goalId] : worldComp->enemyGoalList) {
                    if (i != goalIndex) continue;
                    for (auto& [playerIndex2, teamIndex2, playerId, teamId] : worldComp->playerList) {
                        if (playerIndex1 != 0) if (playerIndex1 != playerIndex2) continue;
                        if (teamIndex1   != 0) if (teamIndex1   != teamIndex2  ) continue;

                        auto goal = entityManager->createEntity(mapEntity->id);
                        goal->addComponent<PositionComponent>();
                        goal->addComponent<CollisionComponent>(8.0f);
                        goal->addComponent<ImmovableComponent>(true, false, true, false);
                        setNewCenteredPosition(goal, x, y);
                        auto goalComp = goal->addComponent<EnemyGoalComponent>();
                        if (playerIndex1 != 0) goalComp->playerID = playerId;
                        if (teamIndex1   != 0) goalComp->teamID   = teamId  ;
                        goalId = goal->id;
                        break;
                    }
                    break;
                }
            }
        }
        
        // Add enemy spawners
        i = 0;
        for (auto& [mapEntity, mapComp] : maps) {
            for (auto& [x, y] : mapComp->enemySpawns) {
                i++;
                for (auto& [spawnIndex, wavesId, goalIndex1] : worldComp->enemySpawnList) {
                    if (i != spawnIndex) continue;
                    for (auto& [goalIndex2, playerIndex, teamIndex, goalId] : worldComp->enemyGoalList) {
                        if (goalIndex1 != goalIndex2) continue;

                        auto spawner = entityManager->createEntity(mapEntity->id);
                        spawner->addComponent<PositionComponent>();
                        spawner->addComponent<CollisionComponent>(16.0f);
                        spawner->addComponent<ImmovableComponent>(true, false, true, false);
                        setNewCenteredPosition(spawner, x, y);
                        auto spawnerComp = spawner->addComponent<EnemySpawnerComponent>();
                        spawnerComp->goalID = goalId;
                        spawnerComp->wavesID = wavesId;
                        break;
                    }
                    break;
                }
            }
        }

        assert(saveReplay);
        saveReplay->initial = std::make_unique<InitialConditions>();
        saveReplay->initial->worldID = worldID;
        saveReplay->initial->playerMapExt = playerMapExt_;

        serialize("startup.txt");
    }

    void Game::startReplay(json& j) {
        from_json(j);
        assert(entityManager);
        thisPlayer = NO_ENTITY;
        auto players = entityManager->getEntitiesWithComponents<PlayerComponent>();
        for(auto& [entity, playerComp] : players) {
            thisPlayer = entity->id;
        }
        assert(thisPlayer != NO_ENTITY);
    }

    void Game::receiveGameState(GameState* gameState) {
        assert(gameState);
        assert(gameState->entityManager);
        assert(gameState->collisionTracker);
        entityManager = std::move(gameState->entityManager);
        collisionTracker = std::move(gameState->collisionTracker);
    }
            
}

#endif
