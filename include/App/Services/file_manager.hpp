#pragma once

#include <Game/constants.hpp>


namespace SupDef {

    struct FileManager {
        FileManager() { }

        void clearData() {
            j_replay = json();
            j_assets = json();
            j_entities = json();
            j_collisions = json();
            j_game_state = json();
            j_mod_header = json();
            j_full_replay = json();

            has_replay = false; has_asset = false; has_entities = false;
            has_collisions = false; has_game_state = false;
            has_mod_header = false; contains_full_replay = false;
        }

        bool loadFromFile(const std::string& filename) {
            clearData();
            std::ifstream file(filename);
            if (!file) {
                std::stringstream ss;
                ss << "Failed to open game file: " << filename;
                LOG_ERROR(ss.str())
                return false;
            }
            try {
                json j;
                file >> j;
                splitFile(j);
            } catch (const nlohmann::json::parse_error& e) {
                std::stringstream ss;
                ss << "JSON parsing error: " << e.what();
                LOG_ERROR(ss.str())
                return false;
            }
            loadFile = filename;
            return true;
        }

        void splitFile(json& j) {
            if (j.contains(SG_ASSET_MANAGER)) {
                j_assets = j[SG_ASSET_MANAGER];
                has_asset = true;
            };
            if (j.contains(S_ASSETS)) {
                j_assets = j;
                has_asset = true;
            }
            if (j.contains(SG_REPLAY)) {
                j_replay = j[SG_REPLAY];
                has_replay = true;
            };
            if (j.contains(SG_GAME_STATE)) {
                j_game_state = j[SG_GAME_STATE];
                has_game_state = true;
                // auto& j_gs = j[SG_GAME_STATE];
                // if (j_gs.contains(SG_ENTITY_MANAGER)) {
                //     j_entities = j_gs[SG_ENTITY_MANAGER];
                //     has_entities = true;
                // };
                // if (j_gs.contains(SG_COLLISION_TRACKER)) {
                //     j_collisions = j_gs[SG_COLLISION_TRACKER];
                //     has_collisions = true;
                // };
            };
            // if (j.contains(SG_ENTITY_MANAGER)) {
            //     j_entities = j[SG_ENTITY_MANAGER];
            //     has_entities = true;
            // };
            // if (j.contains(S_ENTITIES)) {
            //     j_entities = j;
            //     has_entities = true;
            // };
            // if (j.contains(SG_COLLISION_TRACKER)) {
            //     j_collisions = j[SG_COLLISION_TRACKER];
            //     has_collisions = true;
            // };
            if (j.contains(MOD_HEADER_ENTRY)) {
                j_mod_header = j[MOD_HEADER_ENTRY];
                has_mod_header = true;
            };
            build();
        }

        void build() {
            // if (has_replay && has_asset && has_entities && has_collisions) {
            //     j_full_replay[SG_ASSET_MANAGER] = j_assets;
            //     j_full_replay[SG_ENTITY_MANAGER] = j_entities;
            //     j_full_replay[SG_COLLISION_TRACKER] = j_collisions;
            //     j_full_replay[SG_REPLAY] = j_replay;
            //     contains_full_replay = true;
            // }
        }
    
        json j_replay, j_assets, j_game_state, j_entities, j_collisions, j_mod_header, j_full_replay;

        bool has_replay = false, has_asset = false, has_game_state = false,
            has_entities = false, has_collisions = false, has_mod_header = false;
        bool contains_full_replay = false;
        
        std::string loadFile = "";

    };
     
    DEFINE_UNIQUE(FileManager, UFileManager)

}
