#include <Game/game.hpp>
#include <Game/BuildAssets/constants.hpp>

#pragma once


namespace SupDef::BuildAssets {

    void buildPlayerExample(AssetManager* am) {
        auto asset = AssetBuilder::buildPlayer(
            AsData(am, AS_PLAYER_EX, AS_PLAYER_EX_NAME_DE, AS_PLAYER_EX_DESC_DE)
        );
        //
        AssetBuilder::addResource(asset, AS_RES_GOLD, 2500);
        AssetBuilder::addResource(asset, AS_RES_WOOD, 3800);
        AssetBuilder::addInit(asset, AS_COM_TECH_MOVE);
    }

    void buildPlayerExampleNoMove(AssetManager* am) {
        auto asset = AssetBuilder::buildPlayer(
            AsData(am, AS_PLAYER_EX_NO_MOVE, AS_PLAYER_EX_NO_MOVE_NAME_DE, AS_PLAYER_EX_NO_MOVE_DESC_DE)
        );
        //
        AssetBuilder::addResource(asset, AS_RES_GOLD, 2400);
        AssetBuilder::addResource(asset, AS_RES_WOOD, 3600);
        AssetBuilder::addInit(asset, AS_COM_TECH_MOVE);
    }

    void buildPlayers(AssetManager* am) {
        buildPlayerExample(am);
        buildPlayerExampleNoMove(am);
    }

}
