#pragma once

#include <Game/asset_builder.hpp>


namespace SupDef::BuildAssets {

    void buildImmovableUnitI1(AssetManager* am) {
        auto asset = AssetBuilder::buildImmovableUnit(
            AsData(am, AS_UNIT_IMM_1, AS_UNIT_IMM_1_NAME_DE, AS_UNIT_IMM_1_DESC_DE),
            AS_UNIT_IMM_1_TILES_CHECK, AS_UNIT_IMM_1_COL_CHECK, AS_UNIT_IMM_1_OCCUPY, AS_UNIT_IMM_1_IMPASSABLE,
            AS_UNIT_IMM_1_WIDTH, AS_UNIT_IMM_1_HEIGHT
        );
        asset->addComponent<InfluenceableComponent>();
        AssetBuilder::addInit(asset, AS_TECH_ALLOW_BUILD1);
        AssetBuilder::addInit(asset, AS_WEAPON_BASIC);
    }

    void buildImmovableUnitI2(AssetManager* am) {
        auto asset = AssetBuilder::buildImmovableUnit(
            AsData(am, AS_UNIT_IMM_2, AS_UNIT_IMM_2_NAME_DE, AS_UNIT_IMM_2_DESC_DE),
            AS_UNIT_IMM_2_TILES_CHECK, AS_UNIT_IMM_2_COL_CHECK, AS_UNIT_IMM_2_OCCUPY, AS_UNIT_IMM_2_IMPASSABLE,
            AS_UNIT_IMM_2_WIDTH, AS_UNIT_IMM_2_HEIGHT
        );
        asset->addComponent<InfluenceableComponent>();
        AssetBuilder::addInit(asset, AS_TECH_ALLOW_BUILD1);
        AssetBuilder::addInit(asset, AS_WEAPON_BASIC_HOMER);
    }

    void buildImmovableHumanBasic(AssetManager* am) {
        auto asset = AssetBuilder::buildImmovableUnit(
            AsData(am, AS_UNIT_HUMAN_MAIN, AS_UNIT_HUMAN_MAIN_NAME_DE, AS_UNIT_HUMAN_MAIN_DESC_DE),
            AS_UNIT_HUMAN_MAIN_WIDTH, AS_UNIT_HUMAN_MAIN_HEIGHT
        );
        asset->addComponent<InfluenceableComponent>();
        AssetBuilder::addInit(asset, AS_COM_TECH_BUILD_ARCHER_TOWER);
        AssetBuilder::addGraphic(asset, "../data/graphics/castle.png", 1000, true);
    }

    void buildImmovableArcherTower(AssetManager* am) {
        auto asset = AssetBuilder::buildImmovableUnit(
            AsData(am, AS_UNIT_ARCHER_TOWER, AS_UNIT_ARCHER_TOWER_NAME_DE, AS_UNIT_ARCHER_TOWER_DESC_DE),
            AS_UNIT_ARCHER_TOWER_WIDTH, AS_UNIT_ARCHER_TOWER_HEIGHT
        );
        asset->addComponent<InfluenceableComponent>();
        AssetBuilder::addInit(asset, AS_WEAPON_ARCHER);
        AssetBuilder::addGraphic(asset, "../data/graphics/archer.png", 1000, true);
    }

    void buildImmovableUnits(AssetManager* am) {
        buildImmovableUnitI1(am);
        buildImmovableUnitI2(am);
        buildImmovableHumanBasic(am);
        buildImmovableArcherTower(am);
    }

}
