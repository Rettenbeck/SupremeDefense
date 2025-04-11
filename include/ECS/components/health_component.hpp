#pragma once

#include <ECS/component_registry.hpp>
#include <Misc/damage.hpp>


namespace SupDef {
    
    DEFINE_COMPONENT_BEGIN(HealthComponent, SC_HEALTH)
        long health = 0, max_health = 0;

        HealthComponent(int health_) : health(health_), max_health(health_) { addToRegistry(); }

        void applyDamage(Damage* damage) {
            health -= damage->damage_amount;
        }

        bool isDead() { return health <= 0; }

        REFLECT_BEGIN
            REFLECT_FIELD(health)
            REFLECT_FIELD(max_health)
        REFLECT_END
    DEFINE_COMPONENT_END

}
