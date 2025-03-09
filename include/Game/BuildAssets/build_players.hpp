#include <Game/game.hpp>
#include <Game/BuildAssets/constants.hpp>

#pragma once


namespace SupDef::BuildAssets {

    void buildPlayerExample(AssetManager* am) {
        auto asset = AssetBuilder::buildPlayer(
            AsData(am, AS_PLAYER_EX, AS_PLAYER_EX_NAME_DE, AS_PLAYER_EX_DESC_DE)
        );
        //
        AssetBuilder::addResource(asset, AS_RES_GOLD, 250);
        AssetBuilder::addResource(asset, AS_RES_WOOD, 380);
        AssetBuilder::addInit(asset, AS_COM_TECH_MOVE);
    }

    void buildPlayers(AssetManager* am) {
        buildPlayerExample(am);
    }

}
