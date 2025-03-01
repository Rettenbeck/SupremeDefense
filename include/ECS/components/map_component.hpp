#include <ECS/component_registry.hpp>

#pragma once


namespace SupDef {

    struct MapComponent : public Component {
        int width = 0, height = 0;

        MapComponent(int width_, int height_) : width(width_), height(height_) { addToRegistry(); }
        MapComponent() { addToRegistry(); }

        void addToRegistry() {
            ComponentRegistry::registerComponent(getTypeName(), []()
                { return std::make_unique<MapComponent>(); });
        }

        void to_json(json& j) const override {
            j = json{{S_WIDTH, width}, {S_HEIGHT, height}};
        }

        void from_json(const json& j) override {
            j.at(S_WIDTH ).get_to(width );
            j.at(S_HEIGHT).get_to(height);
        }

        std::string getTypeName() const override {
            return SC_MAP;
        }

    };

}
