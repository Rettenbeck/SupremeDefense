#include <ECS/component_registry.hpp>
#include <Misc/damage.hpp>

#pragma once


namespace SupDef {

    struct WeaponComponent  : public Component {
        EntityIDs targets;
        AssetID projectile = NO_ASSET;
        Cooldown current_cooldown = 0, cooldown = 0, original_cooldown = 0;
        UDamage damage = nullptr;

        WeaponComponent() { addToRegistry(); }
        
        WeaponComponent(AssetID projectile_, Damage* damage_, Cooldown cooldown_) {
            assert(damage_);
            projectile = projectile_;
            damage = damage_->clone();
            current_cooldown = cooldown_; cooldown = cooldown_;
            addToRegistry();
        }
        
        void addToRegistry() {
            ComponentRegistry::registerComponent(getTypeName(), []()
                { return std::make_unique<WeaponComponent>(); });
        }

        void to_json(json& j) const override {
            j[S_COOLDOWN         ] = cooldown;
            j[S_CURRENT_COOLDOWN ] = current_cooldown;
            j[S_ORIGINAL_COOLDOWN] = original_cooldown;
            j[S_PROJECTILE       ] = projectile;
            j[S_TARGETS          ] = targets;
        }
    
        void from_json(const json& j) override {
            j.at(S_COOLDOWN         ).get_to(cooldown);
            j.at(S_CURRENT_COOLDOWN ).get_to(current_cooldown);
            j.at(S_ORIGINAL_COOLDOWN).get_to(original_cooldown);
            j.at(S_PROJECTILE       ).get_to(projectile);
            j.at(S_TARGETS          ).get_to(targets);
        }
    
        std::string getTypeName() const override {
            return SC_WEAPON;
        }
        
    };

}
