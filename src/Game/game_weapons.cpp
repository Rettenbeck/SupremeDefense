#ifndef GAME_WEAPONS
#define GAME_WEAPONS

#include <Game/game.hpp>


namespace SupDef {

    void Game::processWeapons() {
        auto weapons = entityManager->getEntitiesWithComponents<
            PositionComponent, CollisionComponent, InfluenceComponent, WeaponComponent>();
        //

        for (auto [entity, pos, col, inf, wpn] : weapons) {
            processWeapon(entity, pos, col, wpn);
        }
    }

    void Game::processWeapon(Entity* entity, PositionComponent* pos, CollisionComponent* col, WeaponComponent* wpn) {
        clearWeapon(wpn);
        determineTargetsForWeapon(entity, wpn);
        sortTargetsForWeapon(entity, wpn);
        shootWeapon(entity, pos, col, wpn);
    }

    void Game::clearWeapon(WeaponComponent* wpn) {
        assert(wpn);
        wpn->targets.clear();
    }

    void Game::determineTargetsForWeapon(Entity* entity, WeaponComponent* wpn) {
        assert(entity);
        assert(wpn);
        auto collisionPartners = findCollisionPartners(entity->id, CG_WEAPON);
        for (auto partnerID : collisionPartners) {
            if (getRelation(entity->id, partnerID) == Relation::Opposing) wpn->addTarget(partnerID);
        }
    }

    void Game::sortTargetsForWeapon(Entity* entity, WeaponComponent* wpn) {
        assert(entity);
        assert(wpn);
        // TODO
    }

    void Game::shootWeapon(Entity* entity, PositionComponent* pos, CollisionComponent* col, WeaponComponent* wpn) {
        assert(entity);
        assert(pos);
        assert(col);
        assert(wpn);
        
        if (wpn->targets.empty()) return;
        auto targetID = wpn->targets[0];
        auto target = entityManager->getEntity(targetID);
        if (!target) return;

        auto mapID = getMapOfEntity(entity->id);
        if (!log_assert(mapID != NO_ENTITY)) return;
        auto owner = getPlayer(entity);
        if (!log_assert(owner)) return;
        if (!log_assert(wpn->projectile != NO_ASSET)) return;

        wpn->cooldown = wpn->original_cooldown;
        assert(wpn->cooldown > 0);
        wpn->current_cooldown -= COOLDOWN_FRAME;

        while(wpn->current_cooldown <= 0) {
            shootWeaponNormal(entity, target, mapID, owner, pos, col, wpn);
        }
    }

    void Game::shootWeaponNormal(Entity* entity, Entity* target, EntityID mapID, Entity* owner,
                PositionComponent* pos, CollisionComponent* col, WeaponComponent* wpn) {
        //

        auto centerTarget = getCenterOfEntity(target);
        auto center = getCenterOfEntity(entity, pos, col);
        wpn->current_cooldown += wpn->cooldown;
        auto projectile = createEntityFromAsset(wpn->projectile, mapID, owner->id, center.x, center.y);
        auto proComp = projectile->getComponent<ProjectileComponent>();
        assert(proComp);
        auto movComp = projectile->getComponent<MovementComponent>();
        assert(movComp);
        proComp->createdBy = entity->id;
        movComp->setVelocityTowardsTarget(center.x, center.y, centerTarget.x, centerTarget.y);
        proComp->damage = wpn->damage->clone();
        proComp->target = target->id;
    }
    
}

#endif
