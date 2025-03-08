#include <Game/game.hpp>
#include <Game/game_actions.cpp>
#include <Game/game_commands.cpp>
#include <Game/game_requirements.cpp>
#include <Game/game_collisions.cpp>
#include <Game/game_movement.cpp>
#include <Game/game_logic.cpp>
#include <Game/game_tech.cpp>
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
        // comProcessor = std::make_unique<CommandProcessor>(globalDispatcher);
        
        auto world = entityManager->createEntity();
        world->addComponent<WorldComponent>();

        Logger::getInstance().addMessage(MessageType::Success, "World initialized!");

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
        // globalDispatcher->subscribe<ConfirmCommandEvent>([this](const SupDef::Events& events) {
        //     for (const auto& event : events) {
        //         const auto& typedEvent = static_cast<const ConfirmCommandEvent&>(*event);
        //         handleConfirmCommand(typedEvent);
        //     }
        // });
    }

    Entity* Game::addMap(AssetID mapAssetID) {
        auto worlds = entityManager->getEntitiesWithComponents<WorldComponent>();
        if(worlds.size() == 0) return nullptr;
        auto world_entry = worlds[0];
        auto world = std::get<0>(world_entry);
        return createEntityFromAsset(mapAssetID, world->id);
    }
    
}
