#pragma once

#include <Game/game.hpp>
#include <Game/BuildAssets/constants.hpp>


namespace SupDef::BuildAssets {

    void buildPlayerRoleAll(AssetManager* am) {
        auto asset = AssetBuilder::buildPlayerRole(
            AsData(am, AS_PLAYER_ROLE_ALL, AS_PLAYER_ROLE_ALL_NAME_DE, AS_PLAYER_ROLE_ALL_DESC_DE)
        );
    }

    void buildPlayerRoles(AssetManager* am) {
        buildPlayerRoleAll(am);
    }

}
