#pragma once

#include <ECS/component_registry.hpp>
#include <Misc/wave.hpp>


namespace SupDef {
    
    DEFINE_COMPONENT_BEGIN(EnemySpawnerComponent, SC_ENEMY_SPAWNER)
        EntityID goal = NO_ENTITY;
        Waves waves;

        REFLECT_BEGIN
            REFLECT_FIELD(goal)
            REFLECT_LIST_UNIQUE(waves, Wave)
        REFLECT_END
    DEFINE_COMPONENT_END

}
