#include <Game/asset_builder.hpp>

#pragma once


namespace SupDef::BuildAssets {

    void buildCommandMove(AssetManager* am) {
        auto techData = std::unique_ptr<TechComponent>();
        techData->applyToAll = true;
        techData->affectsSamePlayer = true;
        techData->filterByComponents = true;
        techData->addRequiredComponents<PositionComponent, MovementComponent, CollisionComponent, SelectableComponent>();

        auto asset = AssetBuilder::buildCommand(
            AsData(am, AS_COM_MOVE, AS_COM_MOVE_NAME_DE, AS_COM_MOVE_DESC_DE),
            AS_COM_TECH_MOVE, techData.get()
        );
        asset->addComponent<MoveCommandComponent>();
    }

    void buildCommandTest1(AssetManager* am) {
        auto techData = std::unique_ptr<TechComponent>();
        techData->applyToParent = true;
        techData->affectsAll = true;

        auto asset = AssetBuilder::buildCommand(
            AsData(am, AS_COM_TEST1, AS_COM_TEST1_NAME_DE, AS_COM_TEST1_DESC_DE),
            AS_COM_TECH_TEST1, techData.get()
        );
        asset->addComponent<BuildCommandComponent>(AS_UNIT_IMM_1);
        auto req = asset->retrieveComponent<RequirementComponent>();
        req->add(std::make_unique<SupDef::Resource>(AS_RES_GOLD, 20));
        req->add(std::make_unique<SupDef::Resource>(AS_RES_WOOD, 30));
        
        auto techComp = asset->getComponent<TechComponent>();
        assert(techComp);
        techComp->applyToParent = true;
        techComp->affectsSamePlayer = true;
    }

    void buildCommandTest2(AssetManager* am) {
        auto techData = std::unique_ptr<TechComponent>();
        techData->applyToParent = true;
        techData->affectsAll = true;

        auto asset = AssetBuilder::buildCommand(
            AsData(am, AS_COM_TEST2, AS_COM_TEST2_NAME_DE, AS_COM_TEST2_DESC_DE),
            AS_COM_TECH_TEST2, techData.get()
        );
        asset->addComponent<BuildCommandComponent>(AS_UNIT_IMM_2);
        auto req = asset->retrieveComponent<RequirementComponent>();
        req->add(std::make_unique<SupDef::Resource>(AS_RES_GOLD, 25));
        req->add(std::make_unique<SupDef::Resource>(AS_RES_WOOD, 35));
        
        auto techComp = asset->getComponent<TechComponent>();
        assert(techComp);
        techComp->applyToParent = true;
        techComp->affectsSamePlayer = true;
    }

    void buildCommands(AssetManager* am) {
        buildCommandMove(am);
        buildCommandTest1(am);
        buildCommandTest2(am);
    }

}
