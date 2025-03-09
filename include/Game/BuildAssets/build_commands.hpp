#include <Game/asset_builder.hpp>

#pragma once


namespace SupDef::BuildAssets {

    void buildCommandMove(AssetManager* am) {
        AssetBuilder::buildCommand(
            AsData(am, AS_COM_MOVE, AS_COM_MOVE_NAME_DE, AS_COM_MOVE_DESC_DE),
            AS_COM_TECH_MOVE
        );
    }

    void buildCommandTest1(AssetManager* am) {
        auto asset = AssetBuilder::buildCommand(
            AsData(am, AS_COM_TEST1, AS_COM_TEST1_NAME_DE, AS_COM_TEST1_DESC_DE),
            AS_COM_TECH_TEST1
        );
        auto req = asset->retrieveComponent<RequirementComponent>();
        req->add(std::make_unique<SupDef::Resource>(AS_RES_GOLD, 20));
        req->add(std::make_unique<SupDef::Resource>(AS_RES_WOOD, 30));
    }

    void buildCommandTest2(AssetManager* am) {
        auto asset = AssetBuilder::buildCommand(
            AsData(am, AS_COM_TEST2, AS_COM_TEST2_NAME_DE, AS_COM_TEST2_DESC_DE),
            AS_COM_TECH_TEST2
        );
        auto req = asset->retrieveComponent<RequirementComponent>();
        req->add(std::make_unique<SupDef::Resource>(AS_RES_GOLD, 25));
        req->add(std::make_unique<SupDef::Resource>(AS_RES_WOOD, 35));
    }

    void buildCommands(AssetManager* am) {
        buildCommandMove(am);
        buildCommandTest1(am);
        buildCommandTest2(am);
    }

}
