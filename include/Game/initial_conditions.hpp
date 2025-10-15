#pragma once

#include <ECS/include.hpp>
#include <Game/constants.hpp>


namespace SupDef {

    class InitialConditions {
        public:
            AssetID worldID = NO_ASSET;
            PlayerMapExt playerMapExt;
            int thisPlayer = -1;
            
            InitialConditions() { }

            void to_json(json& j) const {
                j[SG_WORLD_ID] = worldID;
                for (const auto& [id, type, entityId, name] : playerMapExt) {
                    std::stringstream ss; ss << id;
                    json j_player_data;
                    j_player_data[SG_PLAYER_ID] = id;
                    j_player_data[SG_PLAYER_TYPE] = type;
                    j_player_data[SG_PLAYER_ENT_ID] = entityId;
                    j_player_data[SG_PLAYER_NAME] = name;
                    j[SG_PLAYER_DATA][ss.str()] = j_player_data;
                }
            }

            void from_json(const json& j) {
                playerMapExt.clear();
                worldID = j.at(SG_WORLD_ID);
                auto& j_sub = j[SG_PLAYER_DATA];
                for(auto& [id, j_player_data] : j_sub.items()) {
                    int id_ = j_player_data[SG_PLAYER_ID];
                    AssetID type = j_player_data[SG_PLAYER_TYPE];
                    EntityID ent_id = j_player_data[SG_PLAYER_ENT_ID];
                    std::string name = j_player_data[SG_PLAYER_NAME];
                    playerMapExt.emplace_back(id_, type, ent_id, name);
                }
            }
    
    };
    
    DEFINE_UNIQUE(InitialConditions, UInitialConditions);

}
