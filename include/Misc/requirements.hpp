#pragma once

#include <Misc/requirement.hpp>
#include <Misc/resource.hpp>


namespace SupDef {

    class ResourceRequirement : public Requirement {
        public:
            UResource resource = nullptr;

            ResourceRequirement() {}
        
            ResourceRequirement(ResourceID resourceID) {
                resource = std::make_unique<Resource>(resourceID);
            }
            
            ResourceRequirement(ResourceID resourceID, long amount, float rate, long capacity) {
                resource = std::make_unique<Resource>(resourceID, amount, rate, capacity);
            }
            
            ResourceRequirement(UResource resource_) : resource(std::move(resource_)) {}
            
    };
    
}
