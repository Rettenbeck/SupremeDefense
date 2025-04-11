#pragma once

#include <ECS/component_registry.hpp>


namespace SupDef {

    DEFINE_COMPONENT_BEGIN(ResourceDefComponent, SCA_RESOURCE_DEF)
        bool isAsset() const override { return true; }

        REFLECT_BEGIN
        REFLECT_END
    DEFINE_COMPONENT_END
}
