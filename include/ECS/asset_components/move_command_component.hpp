#include <ECS/component_registry.hpp>

#pragma once


namespace SupDef {

    DEFINE_COMPONENT_BEGIN(MoveCommandComponent, SCA_MOVE_COMMAND)
        bool isAsset() const override { return true; }

        REFLECT_COMPONENT_BEGIN(ThisType)
        REFLECT_COMPONENT_END()
    DEFINE_COMPONENT_END

}
