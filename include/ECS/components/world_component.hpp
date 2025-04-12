#pragma once

#include <ECS/component_registry.hpp>


namespace SupDef {

    // DEFINE_EMPTY_COMPONENT(WorldComponent, SC_WORLD)

    DEFINE_COMPONENT_BEGIN(WorldComponent, SC_WORLD)
        std::vector<int> players;

        REFLECT_BEGIN
            REFLECT_FIELD(cooldown)
            REFLECT_FIELD(current_cooldown)
            REFLECT_FIELD(original_cooldown)
            REFLECT_FIELD(projectile)
            REFLECT_FIELD(targets)
            REFLECT_UNIQUE(damage, Damage)
        REFLECT_END
    DEFINE_COMPONENT_END
    
}
