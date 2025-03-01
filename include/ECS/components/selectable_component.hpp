#include <ECS/component_registry.hpp>

#pragma once


namespace SupDef {

    struct SelectableComponent  : public Component {
        SelectableComponent() { addToRegistry(); }
        
        void addToRegistry() {
            ComponentRegistry::registerComponent(getTypeName(), []()
                { return std::make_unique<SelectableComponent>(); });
        }

        void to_json(json& j) const override {
            //
        }
    
        void from_json(const json& j) override {
            //
        }
    
        std::string getTypeName() const override {
            return SC_SELECTABLE;
        }
        
    };

}
