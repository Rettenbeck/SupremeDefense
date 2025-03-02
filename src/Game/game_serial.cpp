#include <Game/game.hpp>

#pragma once


namespace SupDef {

    void Game::to_json(json& j) const {
        // j[SG_PLAYER_MANAGER] = json();
        // playerManager->to_json(j[SG_PLAYER_MANAGER]);
    
        j[SG_ENTITY_MANAGER] = json();
        entityManager->to_json(j[SG_ENTITY_MANAGER]);
    
        j[SG_ASSET_MANAGER] = json();
        assetManager->to_json(j[SG_ASSET_MANAGER]);

        j[SG_TECH_MANAGER] = json();
        techManager->to_json(j[SG_TECH_MANAGER]);
    }

    void Game::from_json(const json& j) {
        // playerManager->from_json(j[SG_PLAYER_MANAGER]);
        entityManager->from_json(j[SG_ENTITY_MANAGER]);
        assetManager ->from_json(j[SG_ASSET_MANAGER ]);
        techManager  ->from_json(j[SG_TECH_MANAGER  ]);
    }

    void Game::serialize(const std::string& filename) const {
        json j;
        to_json(j);
        std::ofstream file(filename);
        file << j.dump(4);  // Pretty print with 4-space indentation
    }

    void Game::deserialize(const std::string& filename) {
        std::ifstream file(filename);
        json j;
        file >> j;
        from_json(j);
    }

}
