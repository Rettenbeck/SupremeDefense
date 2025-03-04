#include <ECS/constants.hpp>
#include <nlohmann/include.hpp>

#pragma once


namespace SupDef {

    const long INFINITE_CAPACITY = -1;

    class Resource {
        public:
            ResourceID resourceID = "";
            long amount = 0;
            float rate = 0.0;
            long capacity = INFINITE_CAPACITY;
            bool selfCost = false;

            Resource() {}
            Resource(ResourceID resourceID, long amount = 0, float rate = 0.0, long capacity = 0)
                : resourceID(resourceID), amount(amount), rate(rate), capacity(capacity) {}

            void to_json(json& j) const {
                j[SX_RESOURCE] = resourceID;
                j[SX_AMOUNT] = amount;
                if (rate != 0.0) j[SX_RATE] = rate;
                if (capacity != INFINITE_CAPACITY) j[SX_MAX_CAP] = capacity;
                if (selfCost) j[SX_SELF_COST] = selfCost;
            }
            
            void from_json(const json& j) {
                resourceID = j.at(SX_RESOURCE).get<ResourceID>();
                amount = j.at(SX_AMOUNT).get<long>();
                
                if (j.contains(SX_RATE)) rate = j.at(SX_RATE).get<float>();
                else rate = 0.0;
                
                if (j.contains(SX_MAX_CAP)) capacity = j.at(SX_MAX_CAP).get<long>();
                else capacity = INFINITE_CAPACITY;
                
                if (j.contains(SX_SELF_COST)) selfCost = j.at(SX_SELF_COST).get<bool>();
                else selfCost = false;
            }
        
    };

    using UResource = std::unique_ptr<Resource>;
    using Resources = std::vector<UResource>;

}
