#include <Game/game.hpp>
#include <Game/BuildAssets/constants.hpp>

#pragma once


namespace SupDef::BuildAssets {

    void buildResources(AssetManager* am) {

    
        auto as_wood = am->createAsset(AS_RES_WOOD);
        as_wood->addComponent<SupDef::ResourceDefComponent>();
        as_wood->addComponent<SupDef::TextComponent>(
            SupDef::SX_NAME, SupDef::SX_LANG_DE, AS_RES_WOOD_NAME_DE,
            SupDef::SX_DESC, SupDef::SX_LANG_DE, AS_RES_WOOD_DESC_DE
        );
    
    }

    void buildResourceWood(AssetManager* am) {
        auto as = am->createAsset(AS_RES_GOLD);
        as->addComponent<SupDef::ResourceDefComponent>();
        as->addComponent<SupDef::TextComponent>(
            SupDef::SX_NAME, SupDef::SX_LANG_DE, AS_RES_GOLD_NAME_DE,
            SupDef::SX_DESC, SupDef::SX_LANG_DE, AS_RES_GOLD_DESC_DE
        );
    }

}
