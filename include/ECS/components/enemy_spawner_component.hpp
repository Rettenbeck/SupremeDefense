#pragma once

#include <ECS/component_registry.hpp>


namespace SupDef {
    
    DEFINE_COMPONENT_BEGIN(EnemySpawnerComponent, SC_ENEMY_SPAWNER)
        EntityID goalID = NO_ENTITY;
        AssetID wavesID = NO_ASSET;

        EnemySpawnerComponent(AssetID wavesID) : wavesID(wavesID) {
            addToRegistry();
        }

        REFLECT_BEGIN
            REFLECT_FIELD(goalID)
            REFLECT_FIELD(wavesID)
        REFLECT_END
    DEFINE_COMPONENT_END

}
