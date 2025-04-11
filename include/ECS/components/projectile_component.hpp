#pragma once

#include <ECS/component_registry.hpp>
#include <Misc/damage.hpp>


namespace SupDef {

    DEFINE_COMPONENT_BEGIN(ProjectileComponent, SC_PROJECTILE)
        AssetIDs applyTechsOnHit;
        int multiHitCooldown = 0;   // 0 = Can only hit once; 1 = hits every frame; 2 = hits every second frame, etc.
        bool homing = false;
        EntityID target = NO_ENTITY;
        UDamage damage = nullptr;
        EntityID createdBy = NO_ENTITY;

        void addTechOnHit(AssetID assetID) {
            push_back_unique(applyTechsOnHit, assetID);
        }

        void removeTechOnHit(AssetID assetID) {
            remove_all(applyTechsOnHit, assetID);
        }

        REFLECT_COMPONENT_BEGINX
            REFLECT_FIELD(applyTechsOnHit)
            REFLECT_FIELD(multiHitCooldown)
            REFLECT_FIELD(homing)
            REFLECT_FIELD(target)
            REFLECT_UNIQUE(damage, Damage)
        REFLECT_COMPONENT_END()
    DEFINE_COMPONENT_END
}
