#include <ECS/component_registry.hpp>

#pragma once


namespace SupDef {

    struct ResourceDefComponent : public Component {
        // ResourceID resource;
        
        ResourceDefComponent() { addToRegistry(); }
        // ResourceDefComponent(ResourceID resourceID) : resource(resourceID) { addToRegistry(); }
        
        void addToRegistry() {
            ComponentRegistry::registerComponent(getTypeName(), []()
                { return std::make_unique<ResourceDefComponent>(); });
        }

        void to_json(json& j) const override {
            // j = json{{S_RESOURCE, resource}};
        }
    
        void from_json(const json& j) override {
            // j.at(S_RESOURCE).get_to(resource);
        }
    
        std::string getTypeName() const override {
            return SCA_RESOURCE_DEF;
        }

        bool isAsset() const override { return true; }

    };
    
}
