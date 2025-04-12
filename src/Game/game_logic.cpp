#ifndef GAME_LOGIC
#define GAME_LOGIC

#include <Game/game.hpp>


namespace SupDef {

    void Game::update(float deltaTime) {
        toPrint = "";
        removeResolvedCollisions();
        updateWorlds(deltaTime);
        determineCollisions();
        processWeapons();
        processProjectiles();
        processTechs();
        processActions();
        generatePlayerList();
    }

    void Game::updateWorlds(float deltaTime) {
        auto worlds = entityManager->getEntitiesWithComponents<WorldComponent>();
        for(auto& world : worlds) {
            updateWorld(deltaTime, world);
        }
    }

    void Game::updateWorld(float deltaTime, _EntWorld world) {
        auto maps = entityManager->getEntitiesWithComponents<MapComponent, TilesComponent>(std::get<0>(world)->id);
        for(auto map : maps) {
            updateMap(deltaTime, map);
        }
    }

    void Game::updateMap(float deltaTime, _EntMapTiles map) {
        auto movingEntities = entityManager->getEntitiesWithComponents<PositionComponent, MovementComponent, CollisionComponent>(std::get<0>(map)->id);
        updatePositionMass(deltaTime, std::get<2>(map), movingEntities);
        passPositionToChildren(std::get<0>(map));

        auto tilesComp = std::get<2>(map);
        tilesChecker->resetTiles(tilesComp);
        tilesChecker->setTiles(tilesComp, entityManager.get());
    }

}

#endif
