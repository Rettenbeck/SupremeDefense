#include <iostream>
#include <include.hpp>
#include <main_asset.cpp>


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

    auto entityManager = g->getEntityManager();

    auto map = g->addMap(ASSET_TEST_MAP);
    auto mapComp   = map->getComponent<SupDef::MapComponent>();
    auto tilesComp = map->getComponent<SupDef::TilesComponent>();

    auto player = g->createEntityFromAsset(ASSET_PLAYER);
    g->setThisPlayer(player->id);

    auto e1 = g->createEntityFromAsset(ASSET_IMMOVABLE, map->id, 111.0,  52.0);
    auto e2 = g->createEntityFromAsset(ASSET_IMMOVABLE, e1->id ,  70.0, 192.0);
    auto e3 = g->createEntityFromAsset(ASSET_IMMOVABLE, map->id, 170.0, 192.0);
    auto e4 = g->createEntityFromAsset(ASSET_IMMOVABLE, map->id, 470.0,  52.0);


    SupDef::V2 start( 2 * 16, 6 * 16);
    SupDef::V2 goal (12 * 16, 8 * 16);
    auto es = g->createEntityFromAsset(ASSET_MOVER,  e3->id, start.x, start.y);
    auto eg = g->createEntityFromAsset(ASSET_MOVER,  e3->id,  goal.x,  goal.y);
    auto em = g->createEntityFromAsset(ASSET_MOVER, map->id, start.x, start.y);
    

    g->getTilesChecker()->setTilesAfterPlacing(e1, tilesComp);
    g->getTilesChecker()->setTilesAfterPlacing(e2, tilesComp);
    g->getTilesChecker()->setTilesAfterPlacing(e3, tilesComp);
    g->getTilesChecker()->setTilesAfterPlacing(e4, tilesComp);


    auto em_pos = em->getComponent<SupDef::PositionComponent>();
    auto em_mov = em->getComponent<SupDef::MovementComponent>();
    em_mov->setGoal(goal.x, goal.y, em_pos->x, em_pos->y);
    g->updateTempGoal(tilesComp, em);


    auto table = g->getEntityManager()->getEntitiesWithComponents<SupDef::ImmovableComponent, SupDef::PositionComponent>();
    for(auto& t : table) {
        std::cout << "Entity " << (std::get<0>(t))->id << "; " << (std::get<1>(t))->marksTilesImpassable << "; " << (std::get<2>(t))->x << "\n";
    }



    serialize_game(g);

    app->run();

}



SupDef::Game* start_app() {
    auto gameLayer    = std::make_unique<SupDef::GameLayer>();
    auto networkLayer = std::make_unique<SupDef::NetworkLayer>();
    auto replayLayer  = std::make_unique<SupDef::ReplayLayer>();
    auto actionRouter = std::make_unique<SupDef::ActionRouter>();
    auto renderLayer  = std::make_unique<SupDef::RenderLayer>();
    auto guiLayer     = std::make_unique<SupDef::GuiLayer>();

    // -> First GUI, then Network, then Game, then Replay; finally ActionRouter
    gameLayer->setPriority(30);
    networkLayer->setPriority(20);
    replayLayer->setPriority(40);
    actionRouter->setPriority(50);
    guiLayer->setPriority(60);
    renderLayer->setPriority(70);

    auto game = gameLayer.get();
    auto router = actionRouter.get();

    app->addLayer(std::move(gameLayer));
    //app->addLayer(std::move(networkLayer));
    app->addLayer(std::move(replayLayer));
    app->addLayer(std::move(actionRouter));
    app->addLayer(std::move(guiLayer));
    app->addLayer(std::move(renderLayer));

    auto g = game->getGame();

    create_assets(g);

    return g;
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
