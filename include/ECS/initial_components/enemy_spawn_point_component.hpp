#pragma once

#include <ECS/component_registry.hpp>


namespace SupDef {

    DEFINE_COMPONENT_BEGIN(EnemySpawnPointComponent, SCI_ENEMY_SPAWN_POINT)
        bool isAsset() const override { return true; }

        REFLECT_BEGIN
        REFLECT_END
    DEFINE_COMPONENT_END
}
