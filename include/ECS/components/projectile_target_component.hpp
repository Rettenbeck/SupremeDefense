#include <ECS/component_registry.hpp>

#pragma once


namespace SupDef {

    struct ProjectileTargetComponent : public Component {
        ProjectileTargetComponent() { addToRegistry(); }

        void addToRegistry() {
            ComponentRegistry::registerComponent(getTypeName(), []()
                { return std::make_unique<ProjectileTargetComponent>(); });
        }

        void to_json(json& j) const override {
            // j = json{{S_WIDTH, width}, {S_HEIGHT, height}};
        }

        void from_json(const json& j) override {
            // j.at(S_WIDTH ).get_to(width );
        }

        std::string getTypeName() const override {
            return SC_PROJECTILE_TARGET;
        }

    };

}
