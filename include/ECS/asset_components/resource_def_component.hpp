#include <ECS/component_registry.hpp>

#pragma once


namespace SupDef {

    DEFINE_COMPONENT_BEGIN(ResourceDefComponent, SCA_RESOURCE_DEF)
        bool isAsset() const override { return true; }

        REFLECT_COMPONENT_BEGIN(ThisType)
        REFLECT_COMPONENT_END()
    DEFINE_COMPONENT_END
}
