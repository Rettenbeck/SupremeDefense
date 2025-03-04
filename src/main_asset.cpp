#include <iostream>
#include <include.hpp>


const std::string ASSET_PLAYER = "player";
const std::string ASSET_RES_GOLD = "gold";
const std::string ASSET_RES_WOOD = "wood";
const std::string ASSET_TEST_COM = "test_command";
const std::string ASSET_TEST_MAP = "test_map";
const std::string ASSET_IMMOVABLE = "immovable";
const std::string ASSET_MOVER = "mover";



void create_assets(SupDef::Game* g) {
    auto am = g->getAssetManager();

    auto am_player = am->createAsset(ASSET_PLAYER);
    am_player->addComponent<SupDef::PlayerComponent>();
    auto am_player_res = am_player->addComponent<SupDef::ResourceComponent>();
    am_player_res->addResource(ASSET_RES_GOLD, 100);
    am_player_res->addResource(ASSET_RES_WOOD, 250);

    auto am_res_gold = am->createAsset(ASSET_RES_GOLD);
    am_res_gold->addComponent<SupDef::ResourceDefComponent>();
    am_res_gold->addComponent<SupDef::TextComponent>(SupDef::SX_NAME, SupDef::SX_LANG_DE, "Gold", SupDef::SX_DESC, SupDef::SX_LANG_DE, "Wertvolles Gold");

    auto am_res_wood = am->createAsset(ASSET_RES_WOOD);
    am_res_wood->addComponent<SupDef::ResourceDefComponent>();
    am_res_wood->addComponent<SupDef::TextComponent>(SupDef::SX_NAME, SupDef::SX_LANG_DE, "Holz", SupDef::SX_DESC, SupDef::SX_LANG_DE, "Gehacktes Holz");

    auto am_command_test = am->createAsset(ASSET_TEST_COM);
    am_command_test->addComponent<SupDef::CommandComponent>(SupDef::COM_TYPE_BUILD);
    am_res_wood->addComponent<SupDef::TextComponent>(SupDef::SX_NAME, SupDef::SX_LANG_DE, "TestCom", SupDef::SX_DESC, SupDef::SX_LANG_DE, "Testbefehl");
    auto am_command_test_req = am_command_test->addComponent<SupDef::RequirementComponent>();
    am_command_test_req->add(std::make_unique<SupDef::Resource>(ASSET_RES_GOLD, 20));
    am_command_test_req->add(std::make_unique<SupDef::Resource>(ASSET_RES_WOOD, 30));

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
