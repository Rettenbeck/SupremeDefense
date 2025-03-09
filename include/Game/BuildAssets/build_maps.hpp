#include <Game/asset_builder.hpp>

#pragma once


namespace SupDef::BuildAssets {

    void buildMapDefault(AssetManager* am) {
        AssetBuilder::buildMap(
            AsData(am, AS_MAP_EX, AS_MAP_EX_NAME_DE, AS_MAP_EX_DESC_DE),
            DEFAULT_MAP_TILESIZE, 0, 0, DEFAULT_MAP_WIDTH, DEFAULT_MAP_HEIGHT
        );
    }

    void buildMaps(AssetManager* am) {
        buildMapDefault(am);
    }

}
