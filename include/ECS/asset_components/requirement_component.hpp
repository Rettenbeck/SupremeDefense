#include <ECS/component_registry.hpp>
#include <Misc/resource.hpp>

#pragma once


namespace SupDef {

    struct RequirementComponent : public Component {
        Resources resources;

        RequirementComponent() { addToRegistry(); }

        void add(UResource resource) {
            resources.push_back(std::move(resource));
        }
        
        void addToRegistry() {
            ComponentRegistry::registerComponent(getTypeName(), []()
                { return std::make_unique<RequirementComponent>(); });
        }

        void to_json(json& j) const override {
            generic_to_json(j, this);
        }

        void from_json(const json& j) override {
            generic_from_json(j, this);
        }

        REFLECT_COMPONENT_BEGIN(RequirementComponent)
            REFLECT_LIST_UNIQUE(resources, Resource)
        REFLECT_COMPONENT_END()

        // void to_json(json& j) const override {
        //     if (!resources.empty()) {
        //         for (const auto& resource : resources) {
        //             if (resource) {
        //                 json resourceJson;
        //                 resource->to_json(resourceJson);
        //                 resourceJson[S_ID] = resource->resourceID;
        //                 j[S_RESOURCES].push_back(resourceJson);
        //             }
        //         }
        //     }
        // }
    
        // void from_json(const json& j) override {
        //     resources.clear();
        //     if (j.contains(S_RESOURCES) && j[S_RESOURCES].is_array()) {
        //         for (const auto& resourceJson : j[S_RESOURCES]) {
        //             auto resource = std::make_unique<Resource>();
        //             resource->from_json(resourceJson);
        //             resources.push_back(std::move(resource));
        //         }
        //     }
        // }
    
        std::string getTypeName() const override {
            return SC_REQUIREMENT;
        }

        bool isAsset() const override { return true; }

    };
    
}
