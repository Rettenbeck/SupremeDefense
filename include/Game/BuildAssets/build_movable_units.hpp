#pragma once

#include <Game/asset_builder.hpp>


namespace SupDef::BuildAssets {

    void buildMovableUnitM1(AssetManager* am) {
        auto asset = AssetBuilder::buildMovableUnit(
            AsData(am, AS_UNIT_MOVER_1, AS_UNIT_MOVER_1_NAME_DE, AS_UNIT_MOVER_1_DESC_DE),
            AS_UNIT_MOVER_1_HEALTH, AS_UNIT_MOVER_1_SPEED, AS_UNIT_MOVER_1_GROUND_BASED, AS_UNIT_MOVER_1_DUMMY_RADIUS
        );
        asset->addComponent<InfluenceableComponent>();
    }

    void buildMovableUnitM2(AssetManager* am) {
        auto asset = AssetBuilder::buildMovableUnit(
            AsData(am, AS_UNIT_MOVER_2, AS_UNIT_MOVER_2_NAME_DE, AS_UNIT_MOVER_2_DESC_DE),
            AS_UNIT_MOVER_2_HEALTH, AS_UNIT_MOVER_2_SPEED, AS_UNIT_MOVER_2_GROUND_BASED, AS_UNIT_MOVER_2_DUMMY_RADIUS
        );
        asset->addComponent<InfluenceableComponent>();
        AssetBuilder::addInit(asset, AS_COM_TECH_TEST2);
    }

    void buildMovableUnits(AssetManager* am) {
        buildMovableUnitM1(am);
        buildMovableUnitM2(am);
    }

}
