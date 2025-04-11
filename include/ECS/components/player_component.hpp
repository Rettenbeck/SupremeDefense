#pragma once

#include <ECS/component_registry.hpp>


namespace SupDef {

    DEFINE_COMPONENT_BEGIN(PlayerComponent, SC_PLAYER)
        EntityID teamID = NO_ENTITY;
        bool isNeutral = false;
        AssetID starterBuilding = NO_ASSET;

        PlayerComponent(AssetID starterBuilding) : starterBuilding(starterBuilding) {
            addToRegistry();
        }

        REFLECT_BEGIN
            REFLECT_FIELD(teamID)
            REFLECT_FIELD(isNeutral)
            REFLECT_FIELD(starterBuilding)
        REFLECT_END
    DEFINE_COMPONENT_END

}
