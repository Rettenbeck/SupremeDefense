#include <ECS/component_registry.hpp>
#include <ECS/asset_components/resource_def_component.hpp>
#include <Misc/resource.hpp>

#pragma once


namespace SupDef {

    struct ResourceComponent  : public Component {
        std::unordered_map<ResourceID, UResource> resources;
        
        ResourceComponent() { addToRegistry(); }

        void addToRegistry() {
            ComponentRegistry::registerComponent(getTypeName(), []()
                { return std::make_unique<ResourceComponent >(); });
        }

        void addResource(const ResourceID& id, long amount = 0, float gatherRate = 0, long maxCapacity = INFINITE_CAPACITY) {
            resources[id] = std::make_unique<Resource>(id, amount, gatherRate, maxCapacity);
        }

        void addResource(UResource resource) {
            resources[resource->resourceID] = std::move(resource);
        }

        void to_json(json& j) const override {
            for (const auto& [id, resource] : resources) {
                if (resource) {
                    json resourceJson;
                    resource->to_json(resourceJson);
                    j[S_RESOURCES].push_back(resourceJson);
                }
            }
        }
    
        void from_json(const json& j) override {
            resources.clear();
            if (j.contains(S_RESOURCES) && j[S_RESOURCES].is_array()) {
                for (const auto& resourceJson : j[S_RESOURCES]) {
                    auto resource = std::make_unique<Resource>();
                    resource->from_json(resourceJson);
                    resources[resource->resourceID] = std::move(resource);
                }
            }
        }
    
        std::string getTypeName() const override {
            return SC_RESOURCE;
        }
    };
    
}
