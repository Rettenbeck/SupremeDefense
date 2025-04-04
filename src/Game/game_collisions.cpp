#include <Game/game.hpp>

#pragma once


namespace SupDef {

    void Game::determineCollisions() {
        determineCollisionsInfluence();
        determineCollisionsWeapons();
        determineCollisionsProjectiles();
    }

    void Game::determineCollisionsInfluence() {
        determineCollisionsGeneric(
            std::tuple<InfluenceableComponent>{},
            std::tuple<InfluenceComponent, TechComponent>{},
            CG_INFLUENCE, true
        );
    }

    void Game::determineCollisionsWeapons() {
        determineCollisionsGeneric(
            std::tuple<ProjectileTargetComponent>{},
            std::tuple<InfluenceComponent, WeaponComponent>{},
            CG_WEAPON
        );
    }

    void Game::determineCollisionsProjectiles() {
        determineCollisionsGeneric(
            std::tuple<ProjectileComponent>{},
            std::tuple<ProjectileHittableComponent>{},
            CG_PROJECTILE
        );
    }
    
    template <typename... T>
    void Game::determineCollisionsGeneric(CollisionGroup collisionGroup, bool influenceMode) {
        assert(collisionSystem);
        auto group = entityManager->getEntitiesWithComponents<T...>();
        auto group_ents = entityManager->extractEntities(group);
        
        collisionSystem->setInfluenceMode(influenceMode);
        processCollisions(group_ents, collisionGroup);
        collisionSystem->setInfluenceMode(false);
    }

    template<typename... A, typename... B>
    void Game::determineCollisionsGeneric(std::tuple<A...> a, std::tuple<B...> b, CollisionGroup collisionGroup, bool influenceMode) {
        assert(collisionSystem);
        auto group1 = entityManager->getEntitiesWithComponents<A...>();
        auto group2 = entityManager->getEntitiesWithComponents<B...>();
        auto group1_ents = entityManager->extractEntities(group1);
        auto group2_ents = entityManager->extractEntities(group2);
        
        collisionSystem->setInfluenceMode(influenceMode);
        processCollisions(group1_ents, group2_ents, collisionGroup);
        collisionSystem->setInfluenceMode(false);
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
            ss << ent->id << "; x: " << pos->x << "; y: " << pos->y << "; radius: " << col->dummyRadius << "\n";
        }
        ss << "\n";
        ss << "  Group 2:\n    ";
        for (auto [ent, pos, col] : listB) {
            ss << ent->id << "; x: " << pos->x << "; y: " << pos->y << "; radius: " << col->dummyRadius << "\n";
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
    
    VF2 Game::getCenterOfEntity(Entity* entity, PositionComponent* pos, CollisionComponent* col) {
        assert(entity);
        assert(pos);
        if (col) return col->getCenter(pos->x, pos->y);
        return VF2(pos->x, pos->y);
    }

    VF2 Game::getCenterOfEntity(Entity* entity, PositionComponent* pos) {
        assert(entity);
        auto col = entity->getComponent<CollisionComponent>();
        return getCenterOfEntity(entity, pos, col);
    }

    VF2 Game::getCenterOfEntity(Entity* entity) {
        auto pos = entity->getComponent<PositionComponent>();
        auto col = entity->getComponent<CollisionComponent>();
        return getCenterOfEntity(entity, pos, col);
    }

    EntityIDs Game::findCollisionPartners(EntityID entityID, CollisionGroup collisionGroup) {
        EntityIDs result;
        if (entityID == NO_ENTITY) return result;

        auto mapID = getMapOfEntity(entityID);
        auto fullCollisionGroup = buildCollisionGroup(collisionGroup, mapID);
        auto collisions = collisionTracker->retrieve(entityID);
        for (auto collision : collisions) {
            if (collision->collisionGroup == fullCollisionGroup) {
                auto otherID = collision->entityA;
                if (otherID == entityID) otherID = collision->entityB;
                result.push_back(otherID);
            }
        }
        return result;
    }

    PCollisionInfos Game::findCollisionsOfEntity(EntityID entityID, CollisionGroup collisionGroup) {
        PCollisionInfos result;
        if (entityID == NO_ENTITY) return result;

        auto mapID = getMapOfEntity(entityID);
        auto fullCollisionGroup = buildCollisionGroup(collisionGroup, mapID);
        auto collisions = collisionTracker->retrieve(entityID);
        for (auto collision : collisions) {
            if (collision->collisionGroup == fullCollisionGroup) {
                auto otherID = collision->entityA;
                if (otherID == entityID) otherID = collision->entityB;
                collision->entityA = entityID;
                collision->entityB = otherID;
                result.push_back(collision);
            }
        }
        return result;
    }

}
