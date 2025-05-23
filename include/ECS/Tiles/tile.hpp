#pragma once

#include <Util/util.hpp>
#include <ECS/constants.hpp>
#include <nlohmann/include.hpp>


namespace SupDef {

    DEFINE_SCLASS_BEGIN(Tile)
        bool isImpassable = false;
        bool isOccupied   = false;
        float movementCost = TILE_INITIAL_MOVEMENT_COST; // Higher cost = slower movement (e.g., rough terrain)
    
        Tile(float cost) : isImpassable(false), isOccupied(false), movementCost(cost) {}

        Tile(bool impassable, bool occupied, float cost)
            : isImpassable(impassable), isOccupied(occupied), movementCost(cost) {}

        inline void setToDefault() {
            isImpassable = false;
            isOccupied   = false;
        }

        REFLECT_BEGIN
            REFLECT_FIELD(isImpassable)
            REFLECT_FIELD(isOccupied)
            REFLECT_FIELD(movementCost)
        REFLECT_END
    DEFINE_SCLASS_END

    DEFINE_UNIQUE_AND_LIST(Tile, UTile, Tiles);

}
