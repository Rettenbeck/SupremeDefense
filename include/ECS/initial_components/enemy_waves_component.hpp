#pragma once

#include <ECS/component_registry.hpp>
#include <Misc/wave.hpp>


namespace SupDef {

    DEFINE_COMPONENT_BEGIN(EnemyGoalPointComponent, SCI_ENEMY_WAVES)
        Waves waves;

        bool isAsset() const override { return true; }

        REFLECT_BEGIN
            REFLECT_LIST_UNIQUE(waves, Wave)
        REFLECT_END
    DEFINE_COMPONENT_END
}
