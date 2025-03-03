#include <ECS/component_registry.hpp>

#pragma once


namespace SupDef {

    struct ChildrenComponent : public Component {
        EntityIDs children;
    
        ChildrenComponent() { addToRegistry(); }

        void addToRegistry() {
            ComponentRegistry::registerComponent(getTypeName(), []()
                { return std::make_unique<ChildrenComponent>(); });
        }

        void to_json(json& j) const override {
            j = json{{S_CHILDREN, children}};
        }
    
        void from_json(const json& j) override {
            j.at(S_CHILDREN).get_to(children);
        }
    
        std::string getTypeName() const override {
            return SC_CHILDREN;
        }
        
    };
    
}
