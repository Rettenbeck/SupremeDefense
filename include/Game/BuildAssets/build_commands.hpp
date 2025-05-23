#pragma once

#include <Game/asset_builder.hpp>


namespace SupDef::BuildAssets {

    void buildCommandMove(AssetManager* am) {
        auto techComponent = std::make_unique<TechComponent>();
        techComponent->applyToAll = true;
        techComponent->affectsSamePlayer = true;
        techComponent->filterByComponents = true;
        techComponent->addRequiredComponents<PositionComponent, MovementComponent, CollisionComponent, SelectableComponent>();

        auto asset = AssetBuilder::buildCommand(
            AsData(am, AS_COM_MOVE, AS_COM_MOVE_NAME_DE, AS_COM_MOVE_DESC_DE),
            AS_COM_TECH_MOVE, TechData(techComponent.get())
        );
        asset->addComponent<MoveCommandComponent>();
    }

    void buildCommandTest1(AssetManager* am) {
        auto techComponent = std::make_unique<TechComponent>();
        techComponent->applyToOwner = true;
        techComponent->affectsAll = true;

        auto asset = AssetBuilder::buildCommand(
            AsData(am, AS_COM_TEST1, AS_COM_TEST1_NAME_DE, AS_COM_TEST1_DESC_DE),
            AS_COM_TECH_TEST1, TechData(techComponent.get())
        );
        asset->addComponent<BuildCommandComponent>(AS_UNIT_IMM_1);
        AssetBuilder::addRequiredResource(asset, AS_RES_GOLD, 20);
        AssetBuilder::addRequiredResource(asset, AS_RES_WOOD, 30);
    }

    void buildCommandTest2(AssetManager* am) {
        auto techComponent = std::make_unique<TechComponent>();
        techComponent->applyToWithinInfluence = true;
        techComponent->affectsAll = true;

        auto asset = AssetBuilder::buildCommand(
            AsData(am, AS_COM_TEST2, AS_COM_TEST2_NAME_DE, AS_COM_TEST2_DESC_DE),
            AS_COM_TECH_TEST2, TechData(techComponent.get(), AS_COM_TEST2_RADIUS)
        );
        asset->addComponent<BuildCommandComponent>(AS_UNIT_IMM_2);
        AssetBuilder::addRequiredResource(asset, AS_RES_GOLD, 25);
        AssetBuilder::addRequiredResource(asset, AS_RES_WOOD, 35);
    }

    void buildCommandArcherTower(AssetManager* am) {
        auto techComponent = std::make_unique<TechComponent>();
        techComponent->applyToOwner = true;
        techComponent->affectsAll = true;

        auto asset = AssetBuilder::buildCommand(
            AsData(am, AS_COM_BUILD_ARCHER_TOWER, AS_COM_BUILD_ARCHER_TOWER_NAME_DE, ""),
            AS_COM_TECH_BUILD_ARCHER_TOWER, TechData(techComponent.get())
        );
        asset->addComponent<BuildCommandComponent>(AS_UNIT_ARCHER_TOWER);
        AssetBuilder::addRequiredResource(asset, AS_RES_GOLD, 15);
    }

    void buildCommands(AssetManager* am) {
        buildCommandMove(am);
        buildCommandTest1(am);
        buildCommandTest2(am);
        buildCommandArcherTower(am);
    }

}
