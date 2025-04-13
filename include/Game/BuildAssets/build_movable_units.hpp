#pragma once

#include <Game/asset_builder.hpp>


namespace SupDef::BuildAssets {

    void buildMovableUnitM1(AssetManager* am) {
        auto asset = AssetBuilder::buildMovableUnit(
            AsData(am, AS_UNIT_MOVER_1, AS_UNIT_MOVER_1_NAME_DE, AS_UNIT_MOVER_1_DESC_DE),
            AS_UNIT_MOVER_1_HEALTH, AS_UNIT_MOVER_1_SPEED, AS_UNIT_MOVER_1_GROUND_BASED, AS_UNIT_MOVER_1_DUMMY_RADIUS
        );
        asset->addComponent<InfluenceableComponent>();
        AssetBuilder::addGraphic(asset, "../data/graphics/mover1.png", 1000, true, true);
        AssetBuilder::addAnimation(asset, 15, 32, 32, 4);
    }

    void buildMovableUnitM2(AssetManager* am) {
        auto asset = AssetBuilder::buildMovableUnit(
            AsData(am, AS_UNIT_MOVER_2, AS_UNIT_MOVER_2_NAME_DE, AS_UNIT_MOVER_2_DESC_DE),
            AS_UNIT_MOVER_2_HEALTH, AS_UNIT_MOVER_2_SPEED, AS_UNIT_MOVER_2_GROUND_BASED, AS_UNIT_MOVER_2_DUMMY_RADIUS
        );
        asset->addComponent<InfluenceableComponent>();
        AssetBuilder::addInit(asset, AS_COM_TECH_TEST2);
    }

    void buildSpider(AssetManager* am) {
        auto asset = AssetBuilder::buildMovableUnit(
            AsData(am, AS_UNIT_SPIDER, AS_UNIT_SPIDER_NAME_DE, AS_UNIT_SPIDER_DESC_DE),
            AS_UNIT_SPIDER_HEALTH, AS_UNIT_SPIDER_SPEED, AS_UNIT_SPIDER_GROUND_BASED, AS_UNIT_SPIDER_DUMMY_RADIUS
        );
        AssetBuilder::addGraphic(asset, "../data/graphics/spider.png", 1000, true, true);
        AssetBuilder::addAnimation(asset, 2, 46, 46, 2, false, MovementAnimation::ResetAnimationOnStop);
    }

    void buildMovableUnits(AssetManager* am) {
        buildMovableUnitM1(am);
        buildMovableUnitM2(am);
        buildSpider(am);
    }

}
