#pragma once

#include <ECS/component_registry.hpp>
#include <Misc/damage.hpp>


namespace SupDef {

    DEFINE_COMPONENT_BEGIN(WeaponComponent, SC_WEAPON)
        EntityIDs targets;
        AssetID projectile = NO_ASSET;
        Cooldown current_cooldown = 0, cooldown = 0, original_cooldown = 0;
        UDamage damage = nullptr;

        WeaponComponent(AssetID projectile_, Damage* damage_, Cooldown cooldown_) {
            assert(damage_);
            projectile = projectile_;
            damage = damage_->clone();
            original_cooldown = cooldown_; cooldown = cooldown_;
            addToRegistry();
        }
        
        void addTarget(EntityID target) {
            push_back_unique(targets, target);
        }

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
