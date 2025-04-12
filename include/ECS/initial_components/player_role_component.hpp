#pragma once

#include <ECS/component_registry.hpp>


namespace SupDef {

    DEFINE_COMPONENT_BEGIN(PlayerRoleComponent, SCI_PLAYER_ROLE)
        AssetIDs playerList;
        bool selectionBlackListed = false;

        PlayerRoleComponent(AssetIDs playerList, bool blackListed)
        : playerList(playerList), selectionBlackListed(blackListed) {
            addToRegistry();
        }

        bool isAsset() const override { return true; }

        REFLECT_BEGIN
            REFLECT_FIELD(playerList)
            REFLECT_FIELD(selectionBlackListed)
        REFLECT_END
    DEFINE_COMPONENT_END
}
