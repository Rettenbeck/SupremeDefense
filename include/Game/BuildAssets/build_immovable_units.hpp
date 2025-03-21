#include <Game/asset_builder.hpp>

#pragma once


namespace SupDef::BuildAssets {

    void buildImmovableUnitI1(AssetManager* am) {
        auto asset = AssetBuilder::buildImmovableUnit(
            AsData(am, AS_UNIT_IMM_1, AS_UNIT_IMM_1_NAME_DE, AS_UNIT_IMM_1_DESC_DE),
            AS_UNIT_IMM_1_TILES_CHECK, AS_UNIT_IMM_1_COL_CHECK, AS_UNIT_IMM_1_OCCUPY, AS_UNIT_IMM_1_IMPASSABLE,
            AS_UNIT_IMM_1_WIDTH, AS_UNIT_IMM_1_HEIGHT
        );
        AssetBuilder::addInit(asset, AS_COM_TECH_TEST1);
    }

    void buildImmovableUnitI2(AssetManager* am) {
        auto asset = AssetBuilder::buildImmovableUnit(
            AsData(am, AS_UNIT_IMM_2, AS_UNIT_IMM_2_NAME_DE, AS_UNIT_IMM_2_DESC_DE),
            AS_UNIT_IMM_2_TILES_CHECK, AS_UNIT_IMM_2_COL_CHECK, AS_UNIT_IMM_2_OCCUPY, AS_UNIT_IMM_2_IMPASSABLE,
            AS_UNIT_IMM_2_WIDTH, AS_UNIT_IMM_2_HEIGHT
        );
    }

    void buildImmovableUnits(AssetManager* am) {
        buildImmovableUnitI1(am);
        buildImmovableUnitI2(am);
    }

}
