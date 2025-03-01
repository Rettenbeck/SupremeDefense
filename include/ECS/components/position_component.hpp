#include <ECS/component_registry.hpp>

#pragma once


namespace SupDef {

    struct PositionComponent : public Component {
        float x = 0.0f, y = 0.0f;

        PositionComponent(float x_, float y_) : x(x_), y(y_) { addToRegistry(); }
        PositionComponent() { addToRegistry(); }

        void addToRegistry() {
            ComponentRegistry::registerComponent(getTypeName(), []()
                { return std::make_unique<PositionComponent>(); });
        }

        void to_json(json& j) const override {
            j = json{{S_X, x}, {S_Y, y}};
        }

        void from_json(const json& j) override {
            j.at(S_X).get_to(x);
            j.at(S_Y).get_to(y);
        }

        std::string getTypeName() const override {
            return SC_POSITION;
        }

    };

}
