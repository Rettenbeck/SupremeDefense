#pragma once

#include <ECS/component_registry.hpp>


namespace SupDef {
    
    DEFINE_COMPONENT_BEGIN(EnemyGoalSeekerComponent, SC_ENEMY_GOAL_SEEKER)
        EntityID goalID = NO_ENTITY;

        EnemyGoalSeekerComponent(EntityID goalID) : goalID(goalID) {
            addToRegistry();
        }

        REFLECT_BEGIN
            REFLECT_FIELD(goalID)
        REFLECT_END
    DEFINE_COMPONENT_END

}
