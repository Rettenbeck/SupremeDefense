#pragma once

#include <Game/asset_builder.hpp>


namespace SupDef::BuildAssets {

    void buildWorldWinterMaul(AssetManager* am) {
        auto asset = AssetBuilder::buildWorld(
            AsData(am, AS_MAP_WINTER_MAUL, AS_MAP_WINTER_MAUL_NAME_DE, AS_MAP_WINTER_MAUL_DESC_DE),
            AS_MAP_WINTER_MAUL
        );
        AssetBuilder::addWorldPlayer(asset, 1, 0);
        AssetBuilder::addWorldPlayer(asset, 2, 1);
        AssetBuilder::addWorldPlayer(asset, 3, 1);
        AssetBuilder::addWorldEnemySpawn(asset, 1, "w1", 2);
        AssetBuilder::addWorldEnemySpawn(asset, 2, "w2", 1);
        AssetBuilder::addWorldEnemyGoal(asset, 1, 1, 0);
        AssetBuilder::addWorldEnemyGoal(asset, 2, 0, 1);
    }

    void buildWorlds(AssetManager* am) {
        buildWorldWinterMaul(am);
    }

}
