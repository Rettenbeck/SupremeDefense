#include <iostream>
#include <include.hpp>


void test_app();
void test_entities();
void test_events();

SupDef::Game* start_app();
void serialize_game(SupDef::Game* g);
void create_assets (SupDef::Game* g);
auto app = std::make_unique<SupDef::App>();




int main() {
    test_app();
    
    return 0;
}





void test_app() {
    auto g = start_app();
    // auto entityManager = g->getEntityManager();
    
    // auto map = g->addMap(SupDef::AS_MAP_EX);
    // auto mapComp   = map->getComponent<SupDef::MapComponent>();
    // auto tilesComp = map->getComponent<SupDef::TilesComponent>();

    // auto player1 = g->createEntityFromAsset(SupDef::AS_PLAYER_EX);
    // auto player2 = g->createEntityFromAsset(SupDef::AS_PLAYER_EX_NO_MOVE);
    // g->setThisPlayer(player1->id);
    // g->otherPlayer = player2->id;

    // auto e1 = g->createEntityFromAsset(SupDef::AS_UNIT_MOVER_1, map->id, player1->id,  12,  20);
    // auto e2 = g->createEntityFromAsset(SupDef::AS_UNIT_MOVER_1, map->id, player1->id,  14,  64);
    // auto e3 = g->createEntityFromAsset(SupDef::AS_UNIT_MOVER_2, map->id, player1->id,  64,  12);

    // auto e4 = g->createEntityFromAsset(SupDef::AS_UNIT_MOVER_1, map->id, player2->id, 212,  20);
    // auto e5 = g->createEntityFromAsset(SupDef::AS_UNIT_MOVER_1, map->id, player2->id, 214,  64);
    // auto e6 = g->createEntityFromAsset(SupDef::AS_UNIT_MOVER_2, map->id, player2->id, 264,  12);
    
    //serialize_game(g);

    app->run();

}



SupDef::Game* start_app() {
    // auto gameLayer    = std::make_unique<SupDef::GameLayer>();
    auto networkLayer = std::make_unique<SupDef::NetworkLayer>();
    auto replayLayer  = std::make_unique<SupDef::ReplayLayer>();
    auto actionRouter = std::make_unique<SupDef::ActionRouter>();
    auto renderLayer  = std::make_unique<SupDef::RenderLayer>();
    auto guiLayer     = std::make_unique<SupDef::GuiLayer>();

    // auto game = gameLayer.get();
    // auto router = actionRouter.get();

    // app->addLayer(std::move(gameLayer));
    //app->addLayer(std::move(networkLayer));
    app->addLayer(std::move(replayLayer));
    app->addLayer(std::move(actionRouter));
    app->addLayer(std::move(guiLayer));
    app->addLayer(std::move(renderLayer));

    // auto g = game->getGame();
    // SupDef::BuildAssets::build(g->getAssetManager());

    // return g;
    return nullptr;
}

void serialize_game(SupDef::Game* g) {
    auto& logger = SupDef::Logger::getInstance();

    const std::string MSG_SER   = "Serialized to &1"    ;
    const std::string MSG_DESER = "Deserialized from &1";
    std::string current_filename;

    auto g1 = std::make_unique<SupDef::Game>();
    auto g2 = std::make_unique<SupDef::Game>();

    // Serialize to a file
    current_filename = "game_save.txt";
    g->serialize(current_filename);
    logger.addMessage(SupDef::MessageType::Success, MSG_SER  , current_filename);

    // Deserialize and verify
    g1->deserialize(current_filename);
    logger.addMessage(SupDef::MessageType::Success, MSG_DESER, current_filename);

    // Serialize new state to a file
    current_filename = "game_save2.txt";
    g1->serialize(current_filename);
    logger.addMessage(SupDef::MessageType::Success, MSG_SER  , current_filename);

    // Deserialize and verify again
    g2->deserialize(current_filename);
    logger.addMessage(SupDef::MessageType::Success, MSG_DESER, current_filename);

    // Serialize new state to a file again
    current_filename = "game_save3.txt";
    g2->serialize(current_filename);
    logger.addMessage(SupDef::MessageType::Success, MSG_SER  , current_filename);

}
