#include <ECS/component_registry.hpp>

#pragma once


namespace SupDef {

    struct PositionComponent : public Component {
        float x = 0.0f, y = 0.0f;
        float xAbs = 0.0f, yAbs = 0.0f;

        PositionComponent(float x_, float y_) : x(x_), y(y_) { addToRegistry(); }
        PositionComponent() { addToRegistry(); }

        void addToRegistry() {
            ComponentRegistry::registerComponent(getTypeName(), []()
                { return std::make_unique<PositionComponent>(); });
        }

        void to_json(json& j) const override {
            generic_to_json(j, this);
        }

        void from_json(const json& j) override {
            generic_from_json(j, this);
        }

        REFLECT_COMPONENT_BEGIN(PositionComponent)
            REFLECT_FIELD(x)
            REFLECT_FIELD(y)
            REFLECT_FIELD(xAbs)
            REFLECT_FIELD(yAbs)
        REFLECT_COMPONENT_END()

        std::string getTypeName() const override {
            return SC_POSITION;
        }

    };

}
