#include <Game/game.hpp>
#include <Game/BuildAssets/constants.hpp>

#pragma once


namespace SupDef::BuildAssets {

    void buildTechAllowBuildIndirect1(AssetManager* am) {
        auto techComponent = std::make_unique<TechComponent>();
        techComponent->applyToWithinInfluence = true;
        techComponent->affectsAll = true;
        TechData techData(techComponent.get(), AS_TECH_ALLOW_BUILD_INDIRECT1_RADIUS);

        auto asset = AssetBuilder::buildGiftTech(
            AsData(am, AS_TECH_ALLOW_BUILD_INDIRECT1, AS_TECH_ALLOW_BUILD_INDIRECT1_NAME_DE, AS_TECH_ALLOW_BUILD_INDIRECT1_DESC_DE),
            techData, AS_COM_TEST1
        );
    }

    void buildTechs(AssetManager* am) {
        buildTechAllowBuildIndirect1(am);
    }

}
