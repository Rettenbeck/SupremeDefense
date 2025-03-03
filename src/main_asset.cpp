#include <iostream>
#include <include.hpp>


const std::string ASSET_TEST_MAP = "test_map";
const std::string ASSET_IMMOVABLE = "immovable";
const std::string ASSET_MOVER = "mover";



void create_assets(SupDef::Game* g) {
    auto am = g->getAssetManager();

    int tileSize = 16;
    int width  = 640;
    int height = 480;

    auto am_map = am->createAsset(ASSET_TEST_MAP);
    am_map->addComponent<SupDef::MapComponent>(width, height);
    am_map->addComponent<SupDef::TilesComponent>(tileSize, width, height);
    am_map->addComponent<SupDef::TextComponent>(SupDef::SX_NAME, SupDef::SX_LANG_DE, "Map", SupDef::SX_DESC, SupDef::SX_LANG_DE, "Testkarte");

    auto am_imm = am->createAsset(ASSET_IMMOVABLE);
    am_imm->addComponent<SupDef::PositionComponent>(0.0, 0.0);
    am_imm->addComponent<SupDef::ImmovableComponent>(true, false, true, true);
    am_imm->addComponent<SupDef::TextComponent>(SupDef::SX_NAME, SupDef::SX_LANG_DE, "Gebaeude", SupDef::SX_DESC, SupDef::SX_LANG_DE, "Einfaches Gebaeude");
    auto am_imm_col = am_imm->addComponent<SupDef::CollisionComponent>(16.0);
    am_imm_col->addShape(std::make_unique<SupDef::RectangleShape>(32.0, 32.0));

    auto am_mov = am->createAsset(ASSET_MOVER);
    am_mov->addComponent<SupDef::PositionComponent>(0.0, 0.0);
    am_mov->addComponent<SupDef::MovementComponent>(5.0, true);
    am_mov->addComponent<SupDef::CollisionComponent>(6.0);
    am_mov->addComponent<SupDef::TextComponent>(SupDef::SX_NAME, SupDef::SX_LANG_DE, "Einheit", SupDef::SX_DESC, SupDef::SX_LANG_DE, "Einfache Einheit");

}
