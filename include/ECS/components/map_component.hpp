#pragma once

#include <ECS/component_registry.hpp>


namespace SupDef {

    DEFINE_COMPONENT_BEGIN(MapComponent, SC_MAP)
        int width = 0, height = 0;
        MapPlayerSpawnList playerSpawns;
        std::vector<std::tuple<float, float>> enemySpawns, enemyGoals;

        MapComponent(int width_, int height_) : width(width_), height(height_) { addToRegistry(); }

        REFLECT_BEGIN
            REFLECT_FIELD(width)
            REFLECT_FIELD(height)
            REFLECT_FIELD(playerSpawns)
            REFLECT_FIELD(enemySpawns)
            REFLECT_FIELD(enemyGoals)
        REFLECT_END
    DEFINE_COMPONENT_END
    
}
