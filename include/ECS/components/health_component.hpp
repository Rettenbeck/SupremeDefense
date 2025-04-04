#include <ECS/component_registry.hpp>

#pragma once


namespace SupDef {
    
    DEFINE_COMPONENT_BEGIN(HealthComponent, SC_HEALTH)
        int health = 0;

        HealthComponent(int health_) : health(health_) { addToRegistry(); }

        REFLECT_COMPONENT_BEGIN(ThisType)
            REFLECT_FIELD(health)
        REFLECT_COMPONENT_END()
    DEFINE_COMPONENT_END

}
