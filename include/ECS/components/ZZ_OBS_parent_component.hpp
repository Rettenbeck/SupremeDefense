#include <ECS/component_registry.hpp>

#pragma once


namespace SupDef {

    struct ParentComponent : public Component {
        EntityID parent = NO_ENTITY;
        
        ParentComponent() { addToRegistry(); }

        void addToRegistry() {
            ComponentRegistry::registerComponent(getTypeName(), []()
                { return std::make_unique<ParentComponent>(); });
        }

        void to_json(json& j) const override {
            j = json{{S_PARENT, parent}};
        }
    
        void from_json(const json& j) override {
            j.at(S_PARENT).get_to(parent);
        }
    
        std::string getTypeName() const override {
            return SC_PARENT;
        }
    };
    
}
