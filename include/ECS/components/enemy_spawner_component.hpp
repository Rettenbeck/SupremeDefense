#pragma once

#include <ECS/component_registry.hpp>


namespace SupDef {
    
    DEFINE_COMPONENT_BEGIN(EnemySpawnerComponent, SC_ENEMY_SPAWNER)
        EntityID goal = NO_ENTITY;
        AssetID wavesID = NO_ASSET;

        REFLECT_BEGIN
            REFLECT_FIELD(goal)
            REFLECT_FIELD(wavesID)
        REFLECT_END
    DEFINE_COMPONENT_END

}
