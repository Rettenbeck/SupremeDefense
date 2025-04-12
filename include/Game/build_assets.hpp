#pragma once

#include <Game/BuildAssets/build_worlds.hpp>
#include <Game/BuildAssets/build_maps.hpp>
#include <Game/BuildAssets/build_walls.hpp>
#include <Game/BuildAssets/build_players.hpp>
#include <Game/BuildAssets/build_player_roles.hpp>
#include <Game/BuildAssets/build_resources.hpp>
#include <Game/BuildAssets/build_movable_units.hpp>
#include <Game/BuildAssets/build_immovable_units.hpp>
#include <Game/BuildAssets/build_commands.hpp>
#include <Game/BuildAssets/build_techs.hpp>
#include <Game/BuildAssets/build_weapons.hpp>
#include <Game/BuildAssets/build_projectiles.hpp>


namespace SupDef::BuildAssets {

    void build(AssetManager* am) {
        buildWorlds(am);
        buildMaps(am);
        buildWalls(am);
        buildPlayers(am);
        buildPlayerRoles(am);
        buildResources(am);
        buildMovableUnits(am);
        buildImmovableUnits(am);
        buildCommands(am);
        buildTechs(am);
        buildWeapons(am);
        buildProjectiles(am);
    }

}
