#pragma once

#include <Game/game.hpp>
#include <Game/BuildAssets/constants.hpp>


namespace SupDef::BuildAssets {

    void buildProjectileBasic(AssetManager* am) {
        ProjectileData projectileData(
            AS_PROJECTILE_BASIC_MULTIHIT,
            AS_PROJECTILE_BASIC_HOMING,
            AS_PROJECTILE_BASIC_RADIUS,
            AS_PROJECTILE_BASIC_SPEED
        );

        auto asset = AssetBuilder::buildProjectile(
            AsData(am, AS_PROJECTILE_BASIC, AS_PROJECTILE_BASIC_NAME_DE, AS_PROJECTILE_BASIC_DESC_DE),
            projectileData
        );
    }

    void buildProjectileBasicHoming(AssetManager* am) {
        ProjectileData projectileData(
            AS_PROJECTILE_BASIC_HOMER_MULTIHIT,
            AS_PROJECTILE_BASIC_HOMER_HOMING,
            AS_PROJECTILE_BASIC_HOMER_RADIUS,
            AS_PROJECTILE_BASIC_HOMER_SPEED
        );

        auto asset = AssetBuilder::buildProjectile(
            AsData(am, AS_PROJECTILE_BASIC_HOMER, AS_PROJECTILE_BASIC_HOMER_NAME_DE, AS_PROJECTILE_BASIC_HOMER_DESC_DE),
            projectileData
        );
    }

    void buildProjectileArcher(AssetManager* am) {
        ProjectileData projectileData(
            AS_PROJECTILE_ARCHER_MULTIHIT,
            AS_PROJECTILE_ARCHER_HOMING,
            AS_PROJECTILE_ARCHER_RADIUS,
            AS_PROJECTILE_ARCHER_SPEED
        );

        auto asset = AssetBuilder::buildProjectile(
            AsData(am, AS_PROJECTILE_ARCHER, AS_PROJECTILE_ARCHER_NAME_DE, ""),
            projectileData
        );
        AssetBuilder::addGraphic(asset, "../data/graphics/arrow.png", 1000, true, true);
    }

    void buildProjectiles(AssetManager* am) {
        buildProjectileBasic(am);
        buildProjectileBasicHoming(am);
        buildProjectileArcher(am);
    }

}
