#pragma once

#include <Game/game.hpp>
#include <Game/BuildAssets/constants.hpp>


namespace SupDef::BuildAssets {

    void buildWeaponBasic(AssetManager* am) {
        auto damage = std::make_unique<Damage>(AS_WEAPON_BASIC_DAMAGE, AS_WEAPON_BASIC_PIERCE, AS_WEAPON_BASIC_DTYPE);
        auto asset = AssetBuilder::buildWeapon(
            AsData(am, AS_WEAPON_BASIC, AS_WEAPON_BASIC_NAME_DE, AS_WEAPON_BASIC_DESC_DE),
            AS_PROJECTILE_BASIC, damage.get(), AS_WEAPON_BASIC_COOLDOWN, AS_WEAPON_BASIC_RADIUS
        );
    }

    void buildWeaponBasicHoming(AssetManager* am) {
        auto damage = std::make_unique<Damage>(AS_WEAPON_BASIC_HOMER_DAMAGE, AS_WEAPON_BASIC_HOMER_PIERCE, AS_WEAPON_BASIC_HOMER_DTYPE);
        auto asset = AssetBuilder::buildWeapon(
            AsData(am, AS_WEAPON_BASIC_HOMER, AS_WEAPON_BASIC_HOMER_NAME_DE, AS_WEAPON_BASIC_HOMER_DESC_DE),
            AS_PROJECTILE_BASIC_HOMER, damage.get(), AS_WEAPON_BASIC_HOMER_COOLDOWN, AS_WEAPON_BASIC_HOMER_RADIUS
        );
    }

    void buildWeaponArcher(AssetManager* am) {
        auto damage = std::make_unique<Damage>(AS_WEAPON_ARCHER_DAMAGE, AS_WEAPON_ARCHER_PIERCE, AS_WEAPON_ARCHER_DTYPE);
        auto asset = AssetBuilder::buildWeapon(
            AsData(am, AS_WEAPON_ARCHER, AS_WEAPON_ARCHER_NAME_DE, AS_WEAPON_ARCHER_DESC_DE),
            AS_PROJECTILE_ARCHER, damage.get(), AS_WEAPON_ARCHER_COOLDOWN, AS_WEAPON_ARCHER_RADIUS
        );
    }

    void buildWeapons(AssetManager* am) {
        buildWeaponBasic(am);
        buildWeaponBasicHoming(am);
        buildWeaponArcher(am);
    }

}
