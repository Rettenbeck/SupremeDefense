#include <ECS/component_registry.hpp>
#include <Misc/damage.hpp>

#pragma once


namespace SupDef {

    struct ProjectileComponent : public Component {
        AssetIDs applyTechsOnHit;
        int multiHitCooldown = 0;   // 0 = Can only once; 1 = hits every frame; 2 = hits every second frame, etc.
        bool homing = false;
        std::unique_ptr<Damage> damage = nullptr;

        ProjectileComponent() {
            damage = std::make_unique<Damage>();
            addToRegistry();
        }

        void addToRegistry() {
            ComponentRegistry::registerComponent(getTypeName(), []()
                { return std::make_unique<ProjectileComponent>(); });
        }

        void addTechOnHit(AssetID assetID) {
            push_back_unique(applyTechsOnHit, assetID);
        }

        void removeTechOnHit(AssetID assetID) {
            remove_all(applyTechsOnHit, assetID);
        }

        void to_json(json& j) const override {
            j[S_APPLY_TECHS_ON_HIT] = applyTechsOnHit;
            j[S_MULTIHIT_COOLDOWN ] = multiHitCooldown;
            j[S_HOMING] = homing;

            assert(damage);
            j[S_DAMAGE] = json();
            damage->to_json(j[S_DAMAGE]);
        }

        void from_json(const json& j) override {
            j.at(S_APPLY_TECHS_ON_HIT).get_to(applyTechsOnHit );
            j.at(S_MULTIHIT_COOLDOWN ).get_to(multiHitCooldown);
            j.at(S_HOMING).get_to(homing);
            
            assert(damage);
            damage->from_json(j[S_DAMAGE]);
        }

        std::string getTypeName() const override {
            return SC_PROJECTILE;
        }

    };

}
