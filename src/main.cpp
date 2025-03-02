#include <iostream>
#include <include.hpp>

void test_app();
void test_entities();
void test_events();

SupDef::Game* start_app();
void serialize_game(SupDef::Game* g);
auto app = std::make_unique<SupDef::App>();


int main() {
    test_app();
    
    return 0;
}


void test_app() {
    auto g = start_app();

    g->createEmptyMap();
    
    auto entityManager = g->getEntityManager();
    auto techManager = g->getTechManager();

    auto maps = g->getEntityManager()->getEntitiesWithComponents<SupDef::MapComponent, SupDef::TilesComponent>();
    auto map  = std::get<0>(maps.at(0));
    auto mapComp   = std::get<1>(maps.at(0));
    auto tilesComp = std::get<2>(maps.at(0));


    
    auto e1 = entityManager->createEntity(map->id);
    auto e2 = entityManager->createEntity(e1->id);
    auto e3 = entityManager->createEntity(map->id);
    auto e4 = entityManager->createEntity(map->id);

    
    auto es = entityManager->createEntity(e3->id);
    auto eg = entityManager->createEntity(e3->id);
    auto em = entityManager->createEntity(map->id);


    e1->addComponent<SupDef::PositionComponent>(111.0, 52.0);
    e1->addComponent<SupDef::CollisionComponent>(5.0);
    e1->addComponent<SupDef::ImmovableComponent>(true, false, true, true);

    e2->addComponent<SupDef::PositionComponent>(70.0, 192.0);
    e2->addComponent<SupDef::CollisionComponent>(4.0);
    e2->addComponent<SupDef::ImmovableComponent>(true, false, true, true);

    e3->addComponent<SupDef::PositionComponent>(170.0, 192.0);
    e3->addComponent<SupDef::CollisionComponent>(4.0);
    e3->addComponent<SupDef::ImmovableComponent>(true, false, true, true);

    e4->addComponent<SupDef::PositionComponent>(470.0, 192.0);
    e4->addComponent<SupDef::CollisionComponent>(4.0);
    e4->addComponent<SupDef::ImmovableComponent>(true, false, true, true);

    // SupDef::V2 start(6*16, 12*16);
    // SupDef::V2 goal (7*16, 12*16);
    SupDef::V2 start(2*16, 6*16);
    SupDef::V2 goal (12*16, 8*16);
    int testRadius = 7;
    //es->addComponent<SupDef::PositionComponent>(start.x, start.y);
    es->addComponent<SupDef::CollisionComponent>(3.0);
    eg->addComponent<SupDef::PositionComponent>(goal.x, goal.y);
    eg->addComponent<SupDef::CollisionComponent>(3.0);
    
    em->addComponent<SupDef::PositionComponent>(start.x, start.y);
    em->addComponent<SupDef::MovementComponent>(5.0, true);
    em->addComponent<SupDef::CollisionComponent>(6.0);

    auto colComp1 = e1->getComponent<SupDef::CollisionComponent>();
    colComp1->addShape(std::make_unique<SupDef::RectangleShape>(8.0, 220.0));

    auto colComp2 = e2->getComponent<SupDef::CollisionComponent>();
    colComp2->addShape(std::make_unique<SupDef::CircleShape>(12.0));
    colComp2->addShape(std::make_unique<SupDef::RectangleShape>(134.0, 7.0));

    auto colComp3 = e3->getComponent<SupDef::CollisionComponent>();
    colComp3->addShape(std::make_unique<SupDef::RectangleShape>(8.0, 220.0));

    auto colComp4 = e4->getComponent<SupDef::CollisionComponent>();
    colComp4->addShape(std::make_unique<SupDef::RectangleShape>(8.0, 220.0));


    auto colCompS = es->getComponent<SupDef::CollisionComponent>();
    colCompS->addShape(std::make_unique<SupDef::CircleShape>(8.0));
    auto colCompG = eg->getComponent<SupDef::CollisionComponent>();
    colCompG->addShape(std::make_unique<SupDef::CircleShape>(8.0));


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





    
    // using InitEnts = std::vector<std::tuple<float, float, float>>;

    // SupDef::PEntities group1;
    // SupDef::PEntities group2;


    // InitEnts ents1 = {
    //     { 24,  25,  8},
    //     { 33,  28, 27.5},
    //     {455,  26, 25.3},
    //     {457, 423,  9.3},
    //     { 57, 337, 29.3},
    //     {297,  37, 26.2},
    //     {199,  37, 26.2},
    //     {202,  98,  6.2},
    //     {202, 198,  6.2}
    // };

    // InitEnts ents2 = {
    //     { 29,  21, 28},
    //     { 39,  22, 27.5},
    //     {465,  22, 25.3},
    //     {467, 433,  9.3},
    //     { 67, 331, 29.3},
    //     {277,  31, 26.2},
    //     {189,  31, 28.2},
    //     {192,  91,  8.2},
    //     {212, 191,  8.2},
    //     {412, 491,  8.2},
    //     {613, 391,  8.2},
    //     {613, 291,  8.2}
    // };


    // for(const auto& [x, y, r] : ents1) {
    //     auto tmp = g->getEntityManager()->createEntity();
    //     tmp->addComponent<SupDef::PositionComponent>(x, y);
    //     tmp->addComponent<SupDef::CollisionComponent>(r);
    //     group1.push_back(tmp);
    //     std::cout << "G1 Entity " << tmp->id << "; x: " << x << "; y: " << y << "; r: " << r << "\n";
    // }

    // for(const auto& [x, y, r] : ents2) {
    //     // auto tmp = g->getEntityManager()->createEntity();
    //     // tmp->addComponent<SupDef::PositionComponent>(x, y);
    //     // tmp->addComponent<SupDef::CollisionComponent>(r);
    //     // group2.push_back(tmp);
    //     // std::cout << "G2 Entity " << tmp->id << "; x: " << x << "; y: " << y << "; r: " << r << "\n";
    // }


    // auto collisions = g->findCollisions(mapComp, group1);
    // for(auto c : collisions) {
    //     std::cout << "Collision: " << c.a->id << " and " << c.b->id << "\n";
    // }
    // auto collision_stats = g->getCollisionSystem()->getCollisionStats();
    // std::cout << "Collision checks expected: " << std::get<0>(collision_stats) << "\n";
    // std::cout << "Collision checks actual  : " << std::get<1>(collision_stats) << "\n";
    // std::cout << "Collision checks narrow  : " << std::get<2>(collision_stats) << "\n";
    // std::cout << "Collisions total         : " << std::get<3>(collision_stats) << "\n";








    // auto e1 = entityManager->createEntity();
    // auto e2 = entityManager->createEntity();
    // auto e3 = entityManager->createEntity();
    // auto e4 = entityManager->createEntity();

    
    // auto es = entityManager->createEntity();
    // auto eg = entityManager->createEntity();


    // e1->addComponent<SupDef::PositionComponent>(111.0, 22.0);
    // e1->addComponent<SupDef::CollisionComponent>(5.0);
    // e1->addComponent<SupDef::ImmovableComponent>(true, false, true, true);

    // e2->addComponent<SupDef::PositionComponent>(70.0, 192.0);
    // e2->addComponent<SupDef::CollisionComponent>(4.0);
    // e2->addComponent<SupDef::ImmovableComponent>(true, false, true, true);

    // e3->addComponent<SupDef::PositionComponent>(170.0, 192.0);
    // e3->addComponent<SupDef::CollisionComponent>(4.0);
    // e3->addComponent<SupDef::ImmovableComponent>(true, false, true, true);

    // e4->addComponent<SupDef::PositionComponent>(470.0, 192.0);
    // e4->addComponent<SupDef::CollisionComponent>(4.0);
    // e4->addComponent<SupDef::ImmovableComponent>(true, false, true, true);

    // // SupDef::V2 start(6*16, 12*16);
    // // SupDef::V2 goal (7*16, 12*16);
    // SupDef::V2 start(2*16, 2*16);
    // SupDef::V2 goal (5*16, 8*16);
    // int testRadius = 7;
    // es->addComponent<SupDef::PositionComponent>(start.x, start.y);
    // es->addComponent<SupDef::CollisionComponent>(4.0);
    // eg->addComponent<SupDef::PositionComponent>(goal.x, goal.y);
    // eg->addComponent<SupDef::CollisionComponent>(4.0);

    // auto colComp1 = e1->getComponent<SupDef::CollisionComponent>();
    // colComp1->addShape(std::make_unique<SupDef::RectangleShape>(8.0, 220.0));

    // auto colComp2 = e2->getComponent<SupDef::CollisionComponent>();
    // colComp2->addShape(std::make_unique<SupDef::CircleShape>(12.0));
    // colComp2->addShape(std::make_unique<SupDef::RectangleShape>(134.0, 7.0));

    // auto colComp3 = e3->getComponent<SupDef::CollisionComponent>();
    // colComp3->addShape(std::make_unique<SupDef::RectangleShape>(8.0, 220.0));

    // auto colComp4 = e4->getComponent<SupDef::CollisionComponent>();
    // colComp4->addShape(std::make_unique<SupDef::RectangleShape>(8.0, 220.0));


    // auto colCompS = es->getComponent<SupDef::CollisionComponent>();
    // colCompS->addShape(std::make_unique<SupDef::CircleShape>(8.0));
    // auto colCompG = eg->getComponent<SupDef::CollisionComponent>();
    // colCompG->addShape(std::make_unique<SupDef::CircleShape>(8.0));

    // g->getTilesChecker()->setTilesAfterPlacing(e1, tilesComp);
    // g->getTilesChecker()->setTilesAfterPlacing(e2, tilesComp);
    // g->getTilesChecker()->setTilesAfterPlacing(e3, tilesComp);
    // g->getTilesChecker()->setTilesAfterPlacing(e4, tilesComp);


    // SupDef::PEntities group1;
    // SupDef::PEntities group2;

    // // auto group1 = g->getEntityManager()->getEntitiesWithComponents< SupDef::PositionComponent,
    // //                                                                 SupDef::CollisionComponent,
    // //                                                                 SupDef::ImmovableComponent>();
    // //
    // // auto group2 = g->getEntityManager()->getEntitiesWithComponents< SupDef::PositionComponent,
    // //                                                                 SupDef::CollisionComponent,
    // //                                                                 SupDef::ImmovableComponent>();

    // SupDef::PEntities group2;
    // group1.push_back(es);
    // group1.push_back(eg);

    // std::cout << "e1: " << e1->id << "; " << e1 << "\n";
    // std::cout << "e2: " << e2->id << "; " << e2 << "\n";
    // std::cout << "e3: " << e3->id << "; " << e3 << "\n";
    // std::cout << "e4: " << e4->id << "; " << e3 << "\n";
    // std::cout << "es: " << es->id << "; " << es << "\n";
    // std::cout << "eg: " << eg->id << "; " << eg << "\n";

    // auto collisions = g->findCollisions(mapComp, group1);
    // for(auto c : collisions) {
    //     std::cout << "Collision: " << c.a->id << " and " << c.b->id << "\n";
    // }

    // // auto path = g->getPathFinder()->findPathForLargeUnit(tilesComp, start.x, start.y, goal.x, goal.y, testRadius);

    // // for(auto& p : path) {
    // //     auto tmp = entityManager->createEntity();
    // //     tmp->addComponent<SupDef::PositionComponent>(p.x, p.y);
    // //     tmp->addComponent<SupDef::CollisionComponent>(4.0);
    // //     auto colCompP = tmp->getComponent<SupDef::CollisionComponent>();
    // //     colCompP->addShape(std::make_unique<SupDef::CircleShape>(8.0));
    // // }
    
    // serialize_game(g);

    // app->run();

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
    app->addLayer(std::move(networkLayer));
    app->addLayer(std::move(replayLayer));
    app->addLayer(std::move(actionRouter));
    app->addLayer(std::move(guiLayer));
    app->addLayer(std::move(renderLayer));

    auto g = game->getGame();

    auto rules = g->getRules();
    // rules->set("unit_speed", 5.5f);
    // rules->set("max_units", 100);
    // rules->set("is_fog_of_war_enabled", true);
    // rules->set("default_map", "DesertArena");

    // auto action = std::make_shared<SupDef::Action>();
    // g->getGlobalDispatcher()->dispatch<SupDef::ActionCreatedEvent>(action);
    
    auto entityManager = g->getEntityManager();
    auto techManager = g->getTechManager();

    // auto e1 = entityManager->createEntity();
    // auto e2 = entityManager->createEntity();
    // auto e3 = entityManager->createEntity();

    // auto t1 = techManager->createTech(SupDef::TechType::TechOne, json{ "name", 1 }, SupDef::DependencyType::RequiresSource);
    // auto t2 = techManager->createTech(SupDef::TechType::TechOne, json{ "name", 2 }, SupDef::DependencyType::RequiresSource);
    // auto t3 = techManager->createTech(SupDef::TechType::TechOne, json{ "name", 3 }, SupDef::DependencyType::RequiresSource);
    // auto t4 = techManager->createTech(SupDef::TechType::TechOne, json{ "name", 4 }, SupDef::DependencyType::RequiresSource);
    // auto t5 = techManager->createTech(SupDef::TechType::TechOne, json{ "name", 5 }, SupDef::DependencyType::RequiresSource);
    // auto t6 = techManager->createTech(SupDef::TechType::TechOne, json{ "name", 6 }, SupDef::DependencyType::RequiresSource);

    // techManager->linkTechs(t1->id, t2->id);
    // techManager->linkTechs(t2->id, t3->id);
    // techManager->linkTechs(t3->id, t4->id);
    // techManager->linkTechs(t5->id, t4->id);

    // techManager->addOwner(e1->id, t2->id);

    // entityManager->addChild(e1, e2);
    // entityManager->addChild(e1, e3);

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

// void test_entities() {
//     auto entityManager = std::make_unique<SupDef::EntityManager>();

//     auto gameEntity = entityManager->createEntity();
    
//     // auto health = gameEntity->addComponent<SupDef::HealthComponent>();
//     // health->health = 35;


//     auto gameEntitySub1 = entityManager->createEntity();
//     auto health1 = gameEntitySub1->addComponent<SupDef::HealthComponent>();
//     health1->health = 101;

//     auto gameEntitySub2 = entityManager->createEntity();
//     // auto health2 = gameEntitySub2->addComponent<SupDef::HealthComponent>();
//     // health2->health = 122;
    
//     auto gameEntitySub3 = entityManager->createEntity();
//     auto health3 = gameEntitySub3->addComponent<SupDef::HealthComponent>();
//     health3->health = 333;
    

//     auto position1 = gameEntitySub1->addComponent<SupDef::PositionComponent>();
//     position1->x = 10.0f;
//     position1->y = 20.0f;
    
//     auto position2 = gameEntitySub2->addComponent<SupDef::PositionComponent>();
//     position2->x = 14.0f;
//     position2->y = 24.0f;
    


//     entityManager->addChild(gameEntity, gameEntitySub1);
//     entityManager->addChild(gameEntity, gameEntitySub2);
//     entityManager->addChild(gameEntitySub1, gameEntitySub3);


//     auto q1 = entityManager->getEntitiesWithComponents<SupDef::ParentComponent>();
//     auto q2 = entityManager->getEntitiesWithComponents<SupDef::ChildrenComponent>();
//     auto q3 = entityManager->getEntitiesWithComponents<SupDef::ParentComponent, SupDef::ChildrenComponent>();

//     std::cout << "q1:" << "\n";
//     for(auto q : q1) {
//         std::cout << "  id: " << q->id << "\n";
//     }
//     std::cout << "\n";


//     std::cout << "q2:" << "\n";
//     for(auto q : q2) {
//         std::cout << "  id: " << q->id << "\n";
//     }
//     std::cout << "\n";


//     std::cout << "q3:" << "\n";
//     for(auto q : q3) {
//         std::cout << "  id: " << q->id << "\n";
//     }
//     std::cout << "---\n";



//     // // Serialize to a file
//     // entityManager->serialize("game_save.json");
//     // std::cout << "Serialized to game_save.json\n";

//     // // Deserialize and verify
//     // auto loadedManager = std::make_unique<SupDef::EntityManager>();
//     // loadedManager->deserialize("game_save.json");
//     // std::cout << "Deserialized successfully.\n";

//     // // Serialize new state to a file
//     // loadedManager->serialize("game_save2.json");
//     // std::cout << "Serialized to game_save2.json\n";

//     // // Deserialize and verify again
//     // auto loadedManager2 = std::make_unique<SupDef::EntityManager>();
//     // loadedManager2->deserialize("game_save2.json");
//     // std::cout << "Deserialized successfully.\n";

//     // // Serialize new state to a file again
//     // loadedManager2->serialize("game_save3.json");
//     // std::cout << "Serialized to game_save3.json\n";

// }


// void test_events() {
//     // SupDef::EventDispatcher dispatcher;

//     // dispatcher.subscribe<SupDef::UnitDestroyedEvent>([](const SupDef::Events& events) {
//     //     std::cout << "Batch processed: " << events.size() << " UnitDestroyedEvents.\n";
//     //     for (const auto& event : events) {
//     //         const auto& typedEvent = static_cast<const SupDef::UnitDestroyedEvent&>(*event);
//     //         std::cout << "Unit " << typedEvent.unitID << " destroyed.\n";
//     //     }
//     // });
    
//     // // Queue events
//     // dispatcher.queueEvent<SupDef::UnitDestroyedEvent>(42);
//     // dispatcher.queueEvent<SupDef::UnitDestroyedEvent>(43);
    
//     // // Dispatch an immediate event
//     // dispatcher.dispatch<SupDef::UnitDestroyedEvent>(52);
//     // dispatcher.dispatch<SupDef::UnitDestroyedEvent>(53);
    
//     // // Process queued events at the end of the frame
//     // dispatcher.processQueuedEvents();
    
// }


