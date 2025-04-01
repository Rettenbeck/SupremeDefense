#include <ECS/component_registry.hpp>

#pragma once


namespace SupDef {

    struct ProjectileComponent : public Component {
        ProjectileComponent() { addToRegistry(); }

        void addToRegistry() {
            ComponentRegistry::registerComponent(getTypeName(), []()
                { return std::make_unique<ProjectileComponent>(); });
        }

        void to_json(json& j) const override {
            // j = json{{S_HEALTH, health}};
        }

        void from_json(const json& j) override {
            // j.at(S_HEALTH).get_to(health);
        }

        std::string getTypeName() const override {
            return SC_PROJECTILE;
        }

    };

}
