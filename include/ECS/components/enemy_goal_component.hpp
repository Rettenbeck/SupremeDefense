#pragma once

#include <ECS/component_registry.hpp>


namespace SupDef {
    
    DEFINE_COMPONENT_BEGIN(EnemyGoalComponent, SC_ENEMY_GOAL)
        EntityID playerID = NO_ENTITY;
        EntityID teamID = NO_ENTITY;

        EnemyGoalComponent(EntityID playerID, EntityID teamID) : playerID(playerID), teamID(teamID) {
            addToRegistry();
        }

        REFLECT_BEGIN
            REFLECT_FIELD(playerID)
            REFLECT_FIELD(teamID)
        REFLECT_END
    DEFINE_COMPONENT_END

}
