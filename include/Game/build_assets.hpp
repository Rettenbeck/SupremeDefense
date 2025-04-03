#include <Game/BuildAssets/build_maps.hpp>
#include <Game/BuildAssets/build_players.hpp>
#include <Game/BuildAssets/build_resources.hpp>
#include <Game/BuildAssets/build_movable_units.hpp>
#include <Game/BuildAssets/build_immovable_units.hpp>
#include <Game/BuildAssets/build_commands.hpp>
#include <Game/BuildAssets/build_techs.hpp>
#include <Game/BuildAssets/build_weapons.hpp>
#include <Game/BuildAssets/build_projectiles.hpp>

#pragma once


namespace SupDef::BuildAssets {

    void build(AssetManager* am) {
        buildMaps(am);
        buildPlayers(am);
        buildResources(am);
        buildMovableUnits(am);
        buildImmovableUnits(am);
        buildCommands(am);
        buildTechs(am);
        buildWeapons(am);
        buildProjectiles(am);
    }

}
