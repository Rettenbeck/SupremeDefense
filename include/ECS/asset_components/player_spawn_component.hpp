#pragma once

#include <ECS/component_registry.hpp>


namespace SupDef {

    DEFINE_COMPONENT_BEGIN(PlayerSpawnComponent, SCA_PLAYER_SPAWN)
        AssetIDs allowedPlayers;

        PlayerSpawnComponent(AssetID player) {
            allowedPlayers.push_back(player);
            addToRegistry();
        }

        PlayerSpawnComponent(AssetIDs players) {
            allowedPlayers = players;
            addToRegistry();
        }

        bool isAsset() const override { return true; }

        REFLECT_COMPONENT_BEGINX
            REFLECT_FIELD(allowedPlayers)
        REFLECT_COMPONENT_END()
    DEFINE_COMPONENT_END

}
