#pragma once

#include <ECS/constants.hpp>
#include <nlohmann/include.hpp>


namespace SupDef {

    const long INFINITE_CAPACITY = -1;

    DEFINE_SCLASS_BEGIN(Resource)
        ResourceID resourceID = "";
        long amount = 0;
        float rate = 0.0;
        long capacity = INFINITE_CAPACITY;
        bool selfCost = false;

        Resource(ResourceID resourceID, long amount = 0, float rate = 0.0, long capacity = 0)
        : resourceID(resourceID), amount(amount), rate(rate), capacity(capacity) {}

        REFLECT_COMPONENT_BEGINX
            REFLECT_FIELD(resourceID)
            REFLECT_FIELD(amount)
            REFLECT_FIELD(rate)
            REFLECT_FIELD(capacity)
            REFLECT_FIELD(selfCost)
        REFLECT_COMPONENT_END()
    DEFINE_SCLASS_END

    DEFINE_UNIQUE_AND_LIST(Resource, UResource, Resources);

}
