#include <Game/asset_builder.hpp>

#pragma once


namespace SupDef::BuildAssets {

    void buildMapExample(AssetManager* am) {
        AssetBuilder::buildMap(
            AsData(am, AS_MAP_EX, AS_MAP_EX_NAME_DE, AS_MAP_EX_DESC_DE),
            DEFAULT_MAP_TILESIZE, 0, 0, DEFAULT_MAP_WIDTH, DEFAULT_MAP_HEIGHT
        );
    }

    void buildMapWinterMaul(AssetManager* am) {
        AsData mapAsData(am, AS_MAP_WINTER_MAUL, AS_MAP_WINTER_MAUL_NAME_DE, AS_MAP_WINTER_MAUL_DESC_DE);
        auto asset = AssetBuilder::buildMap(
            mapAsData,
            DEFAULT_MAP_TILESIZE, 0, 0, AS_MAP_WINTER_MAUL_WIDTH, AS_MAP_WINTER_MAUL_HEIGHT
        );
        AssetBuilder::addPlayerSpawns(mapAsData, asset, as_map_winter_maul_spawns, AS_PLAYER_EX);
    }

    void buildMaps(AssetManager* am) {
        buildMapExample(am);
        buildMapWinterMaul(am);
    }

}
