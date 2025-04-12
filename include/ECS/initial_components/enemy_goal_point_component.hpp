#pragma once

#include <ECS/component_registry.hpp>


namespace SupDef {

    DEFINE_COMPONENT_BEGIN(EnemyGoalPointComponent, SCI_ENEMY_GOAL_POINT)
        EntityID affectTeam = NO_ENTITY;
        EntityID affectedPlayer = NO_ENTITY;

        bool isAsset() const override { return true; }

        REFLECT_BEGIN
            REFLECT_FIELD(affectTeam)
            REFLECT_FIELD(affectedPlayer)
        REFLECT_END
    DEFINE_COMPONENT_END
}
