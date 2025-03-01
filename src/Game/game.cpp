#include <Game/game.hpp>
#include <Game/game_collisions.cpp>
#include <Game/game_movement.cpp>
#include <Game/game_logic.cpp>
#include <Game/game_rules.cpp>
#include <Game/game_serial.cpp>

#pragma once


namespace SupDef {

    Game::Game() {
        eventDispatcher = std::make_unique<EventDispatcher>();
        //playerManager   = std::make_unique<PlayerManager  >(eventDispatcher.get());
        entityManager   = std::make_unique<EntityManager  >(eventDispatcher.get());
        techManager     = std::make_unique<TechManager    >(eventDispatcher.get());
        tilesChecker    = std::make_unique<TilesChecker   >();
        pathFinder      = std::make_unique<PathFinder     >();
        collisionSystem = std::make_unique<CollisionSystem>();
        rules           = std::make_unique<Rules>();
        
    }

    void Game::initialize() {
        auto world = entityManager->createEntity();
        world->addComponent<WorldComponent>();

        Logger::getInstance().addMessage(MessageType::Success, "World initialized!");

        eventDispatcher->subscribe<EntityDestroyedEvent>([this](const Events& events) {
            std::cout << "Batch processed: " << events.size() << " EntityDestroyedEvents.\n";
            for (const auto& event : events) {
                const auto& typedEvent = static_cast<const EntityDestroyedEvent&>(*event);
                std::cout << "Entity " << typedEvent.entityID << " destroyed.\n";
                this->techManager->onEntityDestroyed(typedEvent.entityID);
            }
        });
    }

    // PEntities Game::getWorlds() { return entityManager->getEntitiesWithComponents<WorldComponent>(); }
    // Entity* Game::getWorld() {
    //     auto worlds = getWorlds();
    //     if(worlds.empty()) return nullptr;
    //     return worlds.at(0);
    // }
    
    EntityID Game::createEmptyMap() {
        auto worlds = entityManager->getEntitiesWithComponents<WorldComponent>();
        if(worlds.size() == 0) return NO_ENTITY;
        auto world = worlds[0];
        auto world_ent = std::get<0>(world);

        int tileSize = 16;
        int width  = 640;
        int height = 480;

        auto map = entityManager->createEntity(world_ent->id);
        map->addComponent<MapComponent>(width, height);
        map->addComponent<TilesComponent>(tileSize, width, height);

        return map->id;
    }
    
}
