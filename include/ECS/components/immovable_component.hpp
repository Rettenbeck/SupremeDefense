#include <ECS/component_registry.hpp>

#pragma once


namespace SupDef {

    struct ImmovableComponent  : public Component {
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

        ImmovableComponent() { addToRegistry(); }

        void addToRegistry() {
            ComponentRegistry::registerComponent(getTypeName(), []()
                { return std::make_unique<ImmovableComponent>(); });
        }

        void to_json(json& j) const override {
            j = json{
                {S_REQUIRES_TILE_CHECK     , requiresTileCheck     },
                {S_REQUIRES_COLLISION_CHECK, requiresCollisionCheck},
                {S_MARKS_TILES_OCCUPIED    , marksTilesOccupied    },
                {S_MARKS_TILES_IMPASSABLE  , marksTilesImpassable  },
                {S_PLACED_DOWN, placedDown}
            };
        }

        void from_json(const json& j) override {
            j.at(S_REQUIRES_TILE_CHECK     ).get_to(requiresTileCheck     );
            j.at(S_REQUIRES_COLLISION_CHECK).get_to(requiresCollisionCheck);
            j.at(S_MARKS_TILES_OCCUPIED    ).get_to(marksTilesOccupied  );
            j.at(S_MARKS_TILES_IMPASSABLE  ).get_to(marksTilesImpassable  );
            j.at(S_PLACED_DOWN).get_to(placedDown);
        }

        std::string getTypeName() const override {
            return SC_IMMOVABLE;
        }

    };

}
