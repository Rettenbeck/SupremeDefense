#ifndef GAME_GAME
#define GAME_GAME

#include <Game/game.hpp>

#ifdef INCLUDE_CPP
    #include <Game/game_startup.cpp>
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
    #include <Game/game_enemies.cpp>
    #include <Game/game_assets.cpp>
    #include <Game/game_serial.cpp>
#endif


namespace SupDef {

    Game::Game() {
        eventDispatcher  = std::make_unique<EventDispatcher >();
        entityManager    = std::make_unique<EntityManager   >();
        assetManager     = std::make_unique<AssetManager    >();
        tilesChecker     = std::make_unique<TilesChecker    >();
        pathFinder       = std::make_unique<PathFinder      >();
        collisionSystem  = std::make_unique<CollisionSystem >();
        collisionTracker = std::make_unique<CollisionTracker>();
        commandTracker   = std::make_unique<CommandTracker  >();
        saveReplay       = std::make_unique<Replay>();
    }

    void Game::initialize() {
        SUBSCRIBE(TriggerCommandEvent)
        SUBSCRIBE(UpdateCommandEvent)
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
        LOG_ERROR("Infinite loop in Game::getMapOfEntity")
        return NO_ENTITY;
    }

    void Game::generatePlayerList() {
        EntityIDs playerList;
        auto players = entityManager->getEntitiesWithComponents<PlayerComponent>();
        for (auto [entity, playerComp] : players) {
            playerList.push_back(entity->id);
        }
        dispatch<SendPlayerListEvent>(playerList);
    }

}

#endif
