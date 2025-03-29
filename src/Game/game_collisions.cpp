#include <Game/game.hpp>

#pragma once


namespace SupDef {

    void Game::determineCollisions() {
        determineCollisionsInfluence();
    }

    void Game::determineCollisionsInfluence() {
        auto influencables = entityManager->getEntitiesWithComponents<InfluenceableComponent>();
        auto influencers   = entityManager->getEntitiesWithComponents<InfluenceComponent>();
        auto influencable_entities = entityManager->extractEntities(influencables);
        auto influence_entities    = entityManager->extractEntities(influencers);
        processCollisions(influencable_entities, influence_entities, CG_INFLUENCE);
    }

    void Game::processCollisions(PEntities group, CollisionGroup collisionGroup) {
        auto list = convertEntitiesForCollision(group);
        for (auto [mapID, entPosCols] : list) {
            auto cg = buildCollisionGroup(collisionGroup, mapID);
            auto map = entityManager->getEntity(mapID);
            assert(map);
            auto mapComp = map->getComponent<MapComponent>();
            assert(mapComp);
            updateCollisions(mapComp, entPosCols, cg);
        }
    }

    void Game::processCollisions(PEntities group1, PEntities group2, CollisionGroup collisionGroup) {
        auto list1 = convertEntitiesForCollision(group1);
        auto list2 = convertEntitiesForCollision(group2);
        for (auto& [mapID, entPosCols1] : list1) {
            if (list2.count(mapID) == 0) continue;
            auto cg = buildCollisionGroup(collisionGroup, mapID);
            auto map = entityManager->getEntity(mapID);
            assert(map);
            auto mapComp = map->getComponent<MapComponent>();
            assert(mapComp);
            auto& entPosCols2 = list2[mapID];
            updateCollisions(mapComp, entPosCols1, entPosCols2, cg);
        }
    }

    _Map_EntPosCols Game::convertEntitiesForCollision(PEntities group) {
        std::unordered_map<EntityID, PEntities> splitByMap;
        _Map_EntPosCols result;
        for (auto entity : group) {
            auto mapID = getMapOfEntity(entity->id);
            splitByMap[mapID].push_back(entity);
        }
        for (auto [mapID, subGroup] : splitByMap) {
            result[mapID] = entityManager->getEntitiesWithComponents<PositionComponent, CollisionComponent>(subGroup);
        }
        return result;
    }

    CollisionGroup Game::buildCollisionGroup(CollisionGroup collisionGroup, EntityID mapID) {
        std::stringstream ss;
        ss << mapID;
        return collisionGroup + ss.str();
    }

    CollisionPairs Game::findCollisions(MapComponent* mapComponent, _EntPosCols& listA, _EntPosCols& listB, bool groupMode) {
        assert(mapComponent);
        Colliders colliders;

        std::stringstream ss;
        ss << "Processing collisions:\n";
        ss << "  Group 1:\n    ";
        for (auto [ent, pos, col] : listA) {
            ss << ent->id << "; x: " << pos->xAbs << "; y: " << pos->yAbs << "; radius: " << col->dummyRadius << "\n";
        }
        ss << "\n";
        ss << "  Group 2:\n    ";
        for (auto [ent, pos, col] : listB) {
            ss << ent->id << "; x: " << pos->xAbs << "; y: " << pos->yAbs << "; radius: " << col->dummyRadius << "\n";
        }
        ss << "\n";
        toPrint += ss.str();

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
        updateCollisions(mapComponent, listA, listB, collisionGroup, true);
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
