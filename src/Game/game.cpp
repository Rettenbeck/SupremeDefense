#include <Game/game.hpp>
#include <Game/game_actions.cpp>
#include <Game/game_commands.cpp>
#include <Game/game_requirements.cpp>
#include <Game/game_collisions.cpp>
#include <Game/game_movement.cpp>
#include <Game/game_player.cpp>
#include <Game/game_logic.cpp>
#include <Game/game_tech.cpp>
#include <Game/game_weapons.cpp>
#include <Game/game_projectiles.cpp>
#include <Game/game_assets.cpp>
#include <Game/game_serial.cpp>

#pragma once


namespace SupDef {

    Game::Game() {
        eventDispatcher  = std::make_unique<EventDispatcher >();
        entityManager    = std::make_unique<EntityManager   >(eventDispatcher.get());
        assetManager     = std::make_unique<AssetManager    >(eventDispatcher.get());
        tilesChecker     = std::make_unique<TilesChecker    >();
        pathFinder       = std::make_unique<PathFinder      >();
        collisionSystem  = std::make_unique<CollisionSystem >();
        collisionTracker = std::make_unique<CollisionTracker>();
        commandTracker   = std::make_unique<CommandTracker  >();
    }

    void Game::initialize() {
        auto world = entityManager->createEntity();
        world->addComponent<WorldComponent>();

        globalDispatcher->subscribe<TriggerCommandEvent>([this](const SupDef::Events& events) {
            for (const auto& event : events) {
                const auto& typedEvent = static_cast<const TriggerCommandEvent&>(*event);
                handleTriggerCommand(typedEvent);
            }
        });
        globalDispatcher->subscribe<UpdateCommandEvent>([this](const SupDef::Events& events) {
            for (const auto& event : events) {
                const auto& typedEvent = static_cast<const UpdateCommandEvent&>(*event);
                handleUpdateCommand(typedEvent);
            }
        });
    }

    Entity* Game::addMap(AssetID mapAssetID) {
        auto worlds = entityManager->getEntitiesWithComponents<WorldComponent>();
        if(worlds.size() == 0) return nullptr;
        auto world_entry = worlds[0];
        auto world = std::get<0>(world_entry);
        return createEntityFromAsset(mapAssetID, world->id);
    }
    
    EntityID Game::getMapOfEntity(EntityID entityID) {
        int count = 50;
        if (entityID == NO_ENTITY) return NO_ENTITY;
        auto current = entityID;
        while(--count > 0) {
            current = entityManager->getParent(current)->id;
            if (current == NO_ENTITY) return NO_ENTITY;
            auto entity = entityManager->getEntity(current);
            assert(entity);
            auto mapComp = entity->getComponent<MapComponent>();
            if (mapComp) return current;
        }
        Logger::getInstance().addMessage(MessageType::Error, "Infinite loop in Game::getMapOfEntity");
        return NO_ENTITY;
    }

}
