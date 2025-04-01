#include <ECS/component_registry.hpp>

#pragma once


namespace SupDef {

    struct InfluenceableComponent : public Component {
        float radius;

        InfluenceableComponent() { addToRegistry(); }

        void addToRegistry() {
            ComponentRegistry::registerComponent(getTypeName(), []()
                { return std::make_unique<InfluenceableComponent>(); });
        }

        void to_json(json& j) const override {
            //
        }

        void from_json(const json& j) override {
            //
        }

        std::string getTypeName() const override {
            return SC_INFLUENCEABLE;
        }

    };

}
