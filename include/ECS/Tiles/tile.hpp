#include <Util/util.hpp>
#include <ECS/constants.hpp>
#include <nlohmann/include.hpp>

#pragma once


namespace SupDef {

    class Tile {
        public:
            bool isImpassable = false;
            bool isOccupied   = false;
            float movementCost; // Higher cost = slower movement (e.g., rough terrain)
        
            Tile(float cost = 1.0f) : isImpassable(false), isOccupied(false), movementCost(cost) {}

            Tile(bool impassable, bool occupied, float cost)
                : isImpassable(impassable), isOccupied(occupied), movementCost(cost) {}

            inline void setToDefault() {
                isImpassable = false;
                isOccupied   = false;
            }

            void to_json(json& j) const {
                generic_to_json(j, this);
            }
    
            void from_json(const json& j) {
                generic_from_json(j, this);
            }
    
            REFLECT_COMPONENT_BEGIN(Tile)
                REFLECT_FIELD(isImpassable)
                REFLECT_FIELD(isOccupied)
                REFLECT_FIELD(movementCost)
            REFLECT_COMPONENT_END()

            // void to_json(json& j) const {
            //     j = json{{S_IS_IMPASSABLE, isImpassable}, {S_IS_OCCUPIED, isOccupied}, {S_MOVEMENT_COST, movementCost}};
            // }
    
            // void from_json(const json& j) {
            //     j.at(S_IS_IMPASSABLE).get_to(isImpassable);
            //     j.at(S_IS_OCCUPIED  ).get_to(isOccupied);
            //     j.at(S_MOVEMENT_COST).get_to(movementCost);
            // }
    
    };

    using UTile = std::unique_ptr<Tile>;
    using Tiles = std::vector<UTile>;

}
