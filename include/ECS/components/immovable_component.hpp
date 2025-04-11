#pragma once

#include <ECS/component_registry.hpp>


namespace SupDef {

    DEFINE_COMPONENT_BEGIN(ImmovableComponent, SC_IMMOVABLE)
        bool requiresTileCheck      = true;   // Check tile occupancy?
        bool requiresCollisionCheck = false;  // Check physical collision?
        bool marksTilesOccupied     = true;   // Should this make tiles occupied?
        bool marksTilesImpassable   = true;   // Should this make tiles impassable?
        bool placedDown = false;

        ImmovableComponent(bool requiresTileCheck_, bool requiresCollisionCheck_, bool marksTilesOccupied_, bool marksTilesImpassable_)
            : requiresTileCheck(requiresTileCheck_),
            requiresCollisionCheck(requiresCollisionCheck_),
            marksTilesOccupied(marksTilesOccupied_),
            marksTilesImpassable(marksTilesImpassable_) { addToRegistry(); }

        REFLECT_COMPONENT_BEGINX
            REFLECT_FIELD(requiresTileCheck)
            REFLECT_FIELD(requiresCollisionCheck)
            REFLECT_FIELD(marksTilesOccupied)
            REFLECT_FIELD(marksTilesImpassable)
            REFLECT_FIELD(placedDown)
        REFLECT_COMPONENT_END()
    DEFINE_COMPONENT_END

}
