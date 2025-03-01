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

    CollisionPairs Game::findCollisions(MapComponent* mapComponent, _EntPosCols& listA, _EntPosCols& listB) {
        return findCollisions(mapComponent, listA, listB, true);
    }

    CollisionPairs Game::findCollisions(MapComponent* mapComponent, _EntPosCols& list) {
        _EntPosCols listB;
        return findCollisions(mapComponent, list, listB, false);
    }

}
