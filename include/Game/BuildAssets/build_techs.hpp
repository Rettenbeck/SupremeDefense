#pragma once

#include <Game/game.hpp>
#include <Game/BuildAssets/constants.hpp>


namespace SupDef::BuildAssets {

    void buildTechAllowBuild1(AssetManager* am) {
        auto techComponent = std::make_unique<TechComponent>();
        techComponent->applyToWithinInfluence = true;
        techComponent->affectsAll = true;
        TechData techData(techComponent.get(), AS_TECH_ALLOW_BUILD1_RADIUS);

        auto asset = AssetBuilder::buildGiftTech(
            AsData(am, AS_TECH_ALLOW_BUILD1, AS_TECH_ALLOW_BUILD1_NAME_DE, AS_TECH_ALLOW_BUILD1_DESC_DE),
            techData, AS_COM_TECH_TEST1
        );
    }

    void buildTechs(AssetManager* am) {
        buildTechAllowBuild1(am);
    }

}
