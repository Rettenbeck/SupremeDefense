#pragma once

#include <ECS/component_registry.hpp>
#include <Misc/resource.hpp>


namespace SupDef {
    
    DEFINE_COMPONENT_BEGIN(RequirementComponent, SC_REQUIREMENT)
        Resources resources;

        void add(UResource resource) {
            resources.push_back(std::move(resource));
        }
        
        bool isAsset() const override { return true; }

        REFLECT_BEGIN
            REFLECT_LIST_UNIQUE(resources, Resource)
        REFLECT_END
    DEFINE_COMPONENT_END
    
}
