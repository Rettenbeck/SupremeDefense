#pragma once

#include <ECS/component_registry.hpp>


namespace SupDef {

    DEFINE_COMPONENT_BEGIN(WorldComponent, SC_WORLD)
        WorldPlayerList playerList;
        WorldEnemySpawnList enemySpawnList;
        WorldEnemyGoalList enemyGoalList;

        REFLECT_BEGIN
            REFLECT_FIELD(playerList)
            REFLECT_FIELD(enemySpawnList)
            REFLECT_FIELD(enemyGoalList)
        REFLECT_END
    DEFINE_COMPONENT_END
    
}
