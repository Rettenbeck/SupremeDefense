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
                for (const auto& [id, type, entitiyId, name] : playerMapExt) {
                    auto& j_sub = j[SG_PLAYER_DATA][id];
                    j_sub[SG_PLAYER_ID] = id;
                    j_sub[SG_PLAYER_TYPE] = type;
                    j_sub[SG_PLAYER_ENT_ID] = entitiyId;
                    j_sub[SG_PLAYER_NAME] = name;
                }
            }

            void from_json(const json& j) {
                playerMapExt.clear();
                worldID = j.at(SG_WORLD_ID);
                auto& j_sub = j[SG_PLAYER_DATA];
                for(auto& [id, j_data] : j_sub.items()) {
                    playerMapExt.emplace_back(
                        j_data[SG_PLAYER_ID],
                        j_data[SG_PLAYER_TYPE],
                        j_data[SG_PLAYER_ENT_ID],
                        j_data[SG_PLAYER_NAME]
                    );
                }
            }
    
    };
    
    DEFINE_UNIQUE(InitialConditions, UInitialConditions);

}
