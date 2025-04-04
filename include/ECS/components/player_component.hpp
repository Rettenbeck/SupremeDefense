#include <ECS/component_registry.hpp>

#pragma once


namespace SupDef {

    DEFINE_COMPONENT_BEGIN(PlayerComponent, SC_PLAYER)
        EntityID teamID = NO_ENTITY;
        bool isNeutral = false;

        REFLECT_COMPONENT_BEGIN(ThisType)
            REFLECT_FIELD(teamID)
            REFLECT_FIELD(isNeutral)
        REFLECT_COMPONENT_END()
    DEFINE_COMPONENT_END

}
