#pragma once

#include <ECS/component_registry.hpp>


namespace SupDef {

    DEFINE_COMPONENT_BEGIN(EnemyGoalPointComponent, SCI_ENEMY_GOAL_POINT)
        bool isAsset() const override { return true; }

        REFLECT_BEGIN
        REFLECT_END
    DEFINE_COMPONENT_END
}
