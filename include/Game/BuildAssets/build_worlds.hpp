#include <Game/asset_builder.hpp>

#pragma once


namespace SupDef::BuildAssets {

    void buildWorldWinterMaul(AssetManager* am) {
        AssetBuilder::buildWorld(
            AsData(am, AS_MAP_WINTER_MAUL, AS_MAP_WINTER_MAUL_NAME_DE, AS_MAP_WINTER_MAUL_DESC_DE),
            AS_MAP_WINTER_MAUL
        );
    }

    void buildWorlds(AssetManager* am) {
        buildWorldWinterMaul(am);
    }

}
