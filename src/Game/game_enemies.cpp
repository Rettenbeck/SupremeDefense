#ifndef GAME_ENEMIES
#define GAME_ENEMIES

#include <Game/game.hpp>


namespace SupDef {

    void Game::processEnemies() {
        auto enemies = entityManager->getEntitiesWithComponents<MovementComponent, EnemyGoalSeekerComponent>();
        for (auto [entity, mov, skr] : enemies) {
            setEnemyMovementTowardsGoal(entity, mov, skr);
        }

        auto spawner = entityManager->getEntitiesWithComponents<PositionComponent, EnemySpawnerComponent>();
        for (auto [entity, pos, spw] : spawner) {
            checkSpawner(entity, pos, spw);
        }
    }

    void Game::setEnemyMovementTowardsGoal(Entity* entity, MovementComponent* mov, EnemyGoalSeekerComponent *skr) {
        assert(entity); assert(mov); assert(skr);
        auto goal = entityManager->getEntity(skr->goalID);
        if (!goal) return;

        auto goalPos = goal->getComponent<PositionComponent>();
        assert(goalPos);
        mov->goalX = goalPos->x;
        mov->goalY = goalPos->y;
        
        auto map = entityManager->getParent(entity->id);
        assert(map);
        auto tilesComp = map->getComponent<TilesComponent>();
        assert(tilesComp);
        mov->setGoal(goalPos->x, goalPos->y, 0, 0);
        updateTempGoal(tilesComp, entity);
    }

    void Game::checkSpawner(Entity* entity, PositionComponent* pos, EnemySpawnerComponent* spw) {
        assert(entity); assert(pos); assert(spw);
        long currentWave = 1;

        auto enemyWaves = assetManager->getAsset(spw->wavesID);
        if (!enemyWaves) {
            LOG_ERROR("Wave component &1 does not exist", spw->wavesID)
            return;
        }

        auto wavesComp = enemyWaves->getComponent<EnemyWavesComponent>();
        assert(wavesComp);
        if (wavesComp->waves.size() < currentWave) return;
        auto& wave = wavesComp->waves[currentWave - 1];
        auto waveSegment = wave->getWaveSegment(frameCount);
        spawnEnemy(pos, waveSegment, spw->goalID, getMapOfEntity(entity->id)) ;
    }

    void Game::spawnEnemy(PositionComponent* pos, WaveSegment* waveSegment, EntityID goalID, EntityID mapID) {
        if (!waveSegment) return;
        auto enemy = createEntityFromAsset(waveSegment->toSpawn, mapID);
        float x = pos->x + waveSegment->xRel;
        float y = pos->y + waveSegment->yRel;
        auto seekerComp = enemy->retrieveComponent<EnemyGoalSeekerComponent>();
        seekerComp->goalID = goalID;
        setNewCenteredPosition(enemy, x, y);
        setInitialOwner(enemy, enemyPlayer);
    }

}

#endif
