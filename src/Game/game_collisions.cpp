#include <Game/game.hpp>

#pragma once


namespace SupDef {

    CollisionPairs Game::findCollisions(MapComponent* mapComponent, _EntPosCols& listA, _EntPosCols& listB, bool groupMode) {
        assert(mapComponent);
        Colliders colliders;

        auto addToColliders = [&](_EntPosCols& list, bool group) {
            for(auto& [entity, pos, col] : list) {
                colliders.push_back(new Collider(entity, pos, col, group));
            }
        };

        addToColliders(listA, false);
        if (groupMode) addToColliders(listB, true);

        return collisionSystem->findCollisions(colliders, mapComponent->width, mapComponent->height, groupMode);
    }

    void Game::updateCollisions(MapComponent* mapComponent, _EntPosCols& listA, _EntPosCols& listB, CollisionGroup collisionGroup, bool groupMode) {
        auto collisions = findCollisions(mapComponent, listA, listB, groupMode);
        collisionTracker->updateCollisions(collisions, collisionGroup);
    }

    void Game::updateCollisions(MapComponent* mapComponent, _EntPosCols& listA, _EntPosCols& listB, CollisionGroup collisionGroup) {
        updateCollisions(mapComponent, listA, listB, collisionGroup, false);
    }

    void Game::updateCollisions(MapComponent* mapComponent, _EntPosCols& list, CollisionGroup collisionGroup) {
        _EntPosCols listB;
        updateCollisions(mapComponent, list, listB, collisionGroup, false);
    }

    void Game::removeResolvedCollisions(CollisionGroup collisionGroup) {
        auto& collisions = collisionTracker->getCollisionList();
        collisions.erase(std::remove_if(collisions.begin(), collisions.end(),
                                        [collisionGroup](const UCollisionInfo& col) { return (col->frameCount < 0) && col->collisionGroup == collisionGroup; }),
                         collisions.end());
    }
    
    void Game::removeResolvedCollisions() {
        auto& collisions = collisionTracker->getCollisionList();
        collisions.erase(std::remove_if(collisions.begin(), collisions.end(),
                                        [](const UCollisionInfo& col) { return col->frameCount < 0; }),
                         collisions.end());
    }
    
}
