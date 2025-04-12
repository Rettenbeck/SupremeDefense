#pragma once

#include <Game/asset_builder.hpp>


namespace SupDef::BuildAssets {

    void buildWallWinter(AssetManager* am) {
        auto asset = AssetBuilder::buildWall(
            AsData(am, AS_WALL_WINTER, AS_WALL_WINTER_NAME_DE, AS_WALL_WINTER_DESC_DE),
            AS_WALL_WINTER_TILES_CHECK, AS_WALL_WINTER_COL_CHECK, AS_WALL_WINTER_OCCUPY, AS_WALL_WINTER_IMPASSABLE,
            AS_WALL_WINTER_WIDTH, AS_WALL_WINTER_HEIGHT
        );
        AssetBuilder::addGraphic(asset, "../data/graphics/wall_winter1.png", 1000, true);
    }

    void buildWalls(AssetManager* am) {
        buildWallWinter(am);
    }

}
