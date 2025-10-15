#ifndef GAME_SERIAL
#define GAME_SERIAL

#include <Game/game.hpp>


namespace SupDef {

    void Game::to_json(json& j) const {
        j[SG_ASSET_MANAGER] = json();
        assetManager->to_json(j[SG_ASSET_MANAGER]);

        j[SG_GAME_STATE][SG_ENTITY_MANAGER] = json();
        entityManager->to_json(j[SG_GAME_STATE][SG_ENTITY_MANAGER]);
    
        j[SG_GAME_STATE][SG_COLLISION_TRACKER] = json();
        collisionTracker->to_json(j[SG_GAME_STATE][SG_COLLISION_TRACKER]);

        j[SG_REPLAY] = json();
        saveReplay->to_json(j[SG_REPLAY]);
    }

    void Game::from_json(const json& j) {
        assetManager     ->from_json(j[SG_ASSET_MANAGER ]);
        entityManager    ->from_json(j[SG_GAME_STATE][SG_ENTITY_MANAGER]);
        collisionTracker ->from_json(j[SG_GAME_STATE][SG_COLLISION_TRACKER]);
        saveReplay       ->from_json(j[SG_REPLAY]);
    }

    void Game::serialize(const std::string& filename) const {
        json j;
        to_json(j);
        std::ofstream file(filename);
        file << j.dump(4);
    }

    void Game::deserialize(const std::string& filename) {
        std::ifstream file(filename);
        json j;
        file >> j;
        from_json(j);
    }

}

#endif
