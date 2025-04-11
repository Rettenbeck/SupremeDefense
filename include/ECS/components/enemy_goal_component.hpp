#pragma once

#include <ECS/component_registry.hpp>


namespace SupDef {
    
    DEFINE_COMPONENT_BEGIN(EnemyGoalComponent, SC_ENEMY_GOAL)
        EntityID goal = NO_ENTITY;

        EnemyGoalComponent(EntityID goal) : goal(goal) {}

        REFLECT_COMPONENT_BEGIN(ThisType)
            REFLECT_FIELD(goal)
        REFLECT_COMPONENT_END()
    DEFINE_COMPONENT_END

}
