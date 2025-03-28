#include <ECS/component_registry.hpp>

#pragma once


namespace SupDef {

    struct InfluenceComponent : public Component {
        float radius;

        InfluenceComponent() { addToRegistry(); }

        void addToRegistry() {
            ComponentRegistry::registerComponent(getTypeName(), []()
                { return std::make_unique<InfluenceComponent>(); });
        }

        void to_json(json& j) const override {
            j[S_RADIUS] = radius;
        }

        void from_json(const json& j) override {
            j.at(S_RADIUS).get_to(radius);
        }

        std::string getTypeName() const override {
            return SC_INFLUENCE;
        }

    };

}
