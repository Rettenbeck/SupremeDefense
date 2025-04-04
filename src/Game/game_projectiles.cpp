#include <Game/game.hpp>

#pragma once


namespace SupDef {

    void Game::processWeapons() {
        auto projectiles = entityManager->getEntitiesWithComponents<
            PositionComponent, MovementComponent, CollisionComponent, ProjectileComponent>();
        //

        for (auto [entity, pos, mov, col, pro] : projectiles) {
            processProjectile(entity, pos, mov, col, pro);
        }
    }
    
    void Game::processProjectile(Entity* entity, PositionComponent* pos, MovementComponent* mov,
        CollisionComponent* col, ProjectileComponent* pro) {
        //
        removeOffMapProjectile(entity, pos);
        processProjectileCollisions(entity, pro);
    }

    void Game::processProjectileCollisions(Entity* projectile, ProjectileComponent* pro) {
        assert(projectile);
        assert(pro);

        auto collision = findCollisionsOfEntity(projectile->id, CG_PROJECTILE);
        for (auto collision : collision) {
            auto partner = entityManager->getEntity(collision->entityB);
            if (!partner) continue;

            auto relation = getRelation(entity->id, partner->id);
            if (relation == Relation::Neutral || relation == Relation::Opposing) {
                if (collision->hasEnded()) continue;
                processProjectileCollision(projectile, pro, partner, collision);
            }
        }
    }

    void Game::processProjectileCollision(Entity* projectile, ProjectileComponent* pro, Entity* other, CollisionInfo* collision) {
        assert(other);
        if (!pro->damage) return;
        auto healthComp = other->getComponent<HealthComponent>();

        bool relevant = false;
        if (pro->multiHitCooldown == 0) {
            if (collision->isNew()) {
                relevant = true;
            }
        } else if (pro->multiHitCooldown == 1) {
            relevant = true;
        } else if (pro->multiHitCooldown > 1) {
            if ((collision->frameCount - 1) % pro->multiHitCooldown == 0) {
                relevant = true;
            }
        }

        if (!relevant) return;

        if (!healthComp && pro->damage->pierce != INFINITE_PIERCE) {
            removeProjectile(projectile->id);
            return;
        }

        healthComp->applyDamage(pro->damage);
        if (healthComp->isDead()) {
            targetDestroyed(other->id);
            other = nullptr;
        }

        if (pro->damage->pierce != INFINITE_PIERCE) {
            pro->damage->pierce--;
            if (pro->damage->pierce < 1) {
                removeProjectile(projectile->id);
                return;
            }
        }
    }

    void Game::removeProjectile(EntityID projectileID) {
        entityManager->removeEntity(projectileID);
    }

    void Game::targetDestroyed(EntityID targetID) {
        entityManager->removeEntity(targetID);
    }

    void Game::removeOffMapProjectile(Entity* entity, PositionComponent* pos, CollisionComponent* col) {
        assert(entity);
        assert(pos);
        assert(col);
        auto mapID = getMapOfEntity(entity);
        auto map = entityManager->getEntity(mapID);
        assert(map);
        auto mapPos = map->getComponent<PositionComponent>();
        auto mapComp = map->getComponent<MapComponent>();
        assert(mapPos);
        assert(mapComp);

        float px1, py1, px2, py2, mx1, my1, mx2, my2;
        px1 = pos->x + col->boundingBox.x;
        py1 = pos->y + col->boundingBox.y;
        px2 = px1 + col->boundingBox.w;
        py2 = py2 + col->boundingBox.h;
        mx1 = mapPos->x;
        my1 = mapPos->y;
        mx2 = mx1 + mapComp->width;
        my2 = my2 + mapComp->height;

        if (px2 <= mx1 || px1 >= mx2 || py2 <= my1 || py1 >= my2) {
            removeProjectile(entity->id);
        }
    }

}