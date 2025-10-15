#pragma once

#include <Game/collision_tracker.hpp>


namespace SupDef {

    struct GameState {
        UEntityManager entityManager;
        UCollisionTracker collisionTracker;
        
        GameState() {}

        void to_json(json& j) const {
            entityManager->to_json(j[SG_ENTITY_MANAGER]);
            collisionTracker->to_json(j[SG_COLLISION_TRACKER]);
        }

        void from_json(const json& j) {
            entityManager = std::make_unique<EntityManager>();
            collisionTracker = std::make_unique<CollisionTracker>();
            entityManager->from_json(j[SG_ENTITY_MANAGER]);
            collisionTracker->from_json(j[SG_COLLISION_TRACKER]);
        }

    };

    DEFINE_UNIQUE(GameState, UGameState)

}
