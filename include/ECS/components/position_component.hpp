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
            j = json{{S_X, x}, {S_Y, y}, {S_X_ABS, xAbs}, {S_Y_ABS, yAbs}};
        }

        void from_json(const json& j) override {
            j.at(S_X).get_to(x);
            j.at(S_Y).get_to(y);
            j.at(S_X_ABS).get_to(xAbs);
            j.at(S_Y_ABS).get_to(yAbs);
        }

        std::string getTypeName() const override {
            return SC_POSITION;
        }

    };

}
