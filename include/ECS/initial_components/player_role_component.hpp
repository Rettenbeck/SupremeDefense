#pragma once

#include <ECS/component_registry.hpp>


namespace SupDef {

    DEFINE_COMPONENT_BEGIN(PlayerRoleComponent, SCI_PLAYER_ROLE)
        bool selectionBlackListed = false;
        AssetIDs playerList;

        bool isAsset() const override { return true; }

        REFLECT_BEGIN
            REFLECT_FIELD(selectionBlackListed)
            REFLECT_FIELD(playerList)
        REFLECT_END
    DEFINE_COMPONENT_END
}
