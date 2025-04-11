#pragma once

#include <ECS/component_registry.hpp>
#include <ECS/asset_components/resource_def_component.hpp>
#include <Misc/resource.hpp>


namespace SupDef {

    DEFINE_COMPONENT_BEGIN(ResourceComponent, SC_RESOURCE)
        std::unordered_map<ResourceID, UResource> resources;
        
        void addResource(const ResourceID& id, long amount = 0, float gatherRate = 0, long maxCapacity = INFINITE_CAPACITY) {
            resources[id] = std::make_unique<Resource>(id, amount, gatherRate, maxCapacity);
        }

        void addResource(UResource resource) {
            resources[resource->resourceID] = std::move(resource);
        }

        REFLECT_COMPONENT_BEGINX
            REFLECT_MAP_UNIQUE(resources, ResourceID, Resource)
        REFLECT_COMPONENT_END()
    DEFINE_COMPONENT_END
    
}
