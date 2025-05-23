#ifndef GAME_MOVE
#define GAME_MOVE

#include <Game/game.hpp>


namespace SupDef {

    void Game::updateTempGoalMass(TilesComponent* tilesComp, _EntPosMovCols& comps) {
        for(auto& comp : comps) {
            updateTempGoal(tilesComp, comp);
        }
    }

    void Game::updateTempGoal(TilesComponent* tilesComp, _EntPosMovCol comp) {
        auto positionComp  = std::get<1>(comp);
        auto movementComp  = std::get<2>(comp);
        auto collisionComp = std::get<3>(comp);
        assert(tilesComp); assert(positionComp); assert(movementComp); assert(collisionComp);

        if (movementComp->movementMode != MovementMode::PursueTarget) return;

        if (!movementComp->hasGoal) {
            movementComp->setVelocityToZero();
            return;
        }
        
        pathTiles.clear();
        auto result = pathFinder->findPathForUnit(  tilesComp, positionComp->x, positionComp->y,
                                                    movementComp->goalX, movementComp->goalY, collisionComp->dummyRadius);
        //

        if (!result->path.empty()) {
            for (auto& p : result->path) {
                pathTiles.emplace_back(p->x, p->y);
            }
        }

        if (result->stuck || !result->found || result->path.empty()) {
            movementComp->clearGoal();
            movementComp->setVelocityToZero();
        } else {
            int index = 0;
            if (result->path.size() > 1) index = 1;
            auto& p = result->path[index];
            movementComp->tempGoalX = p->x;
            movementComp->tempGoalY = p->y;
            movementComp->setVelocityTowardsGoal(positionComp->x, positionComp->y);
        }
    }
    
    void Game::updateTempGoal(TilesComponent* tilesComp, Entity* entity) {
        auto pos = entity->getComponent<PositionComponent>();
        auto mov = entity->getComponent<MovementComponent>();
        auto col = entity->getComponent<CollisionComponent>();
        assert(pos); assert(mov); assert(col);
        updateTempGoal(tilesComp, _EntPosMovCol(nullptr, pos, mov, col));
    }

    void Game::updatePositionMass(float deltaTime, TilesComponent* tilesComp, _EntPosMovCols& comps) {
        for(auto& comp : comps) {
            updatePosition(deltaTime, tilesComp, comp);
        }
    }

    void Game::updatePosition(float deltaTime, TilesComponent* tilesComp, _EntPosMovCol comp) {
        auto positionComp  = std::get<1>(comp);
        auto movementComp  = std::get<2>(comp);
        auto collisionComp = std::get<3>(comp);
        assert(tilesComp); assert(positionComp); assert(movementComp); assert(collisionComp);

        positionComp->storeOldPosition();
        if (movementComp->movementMode == MovementMode::DirectedMotion) {
            updatePositionDirected(deltaTime, tilesComp, comp);
            return;
        }

        if (!movementComp->hasGoal) {
            movementComp->setVelocityToZero();
            return;
        }
    
        // movementComp->speed = movementComp->original_speed;
        float newX = positionComp->x + movementComp->vx * deltaTime;
        float newY = positionComp->y + movementComp->vy * deltaTime;

        float targetX = movementComp->isGroundBased ? movementComp->tempGoalX : movementComp->goalX;
        float targetY = movementComp->isGroundBased ? movementComp->tempGoalY : movementComp->goalY;
        
        float distanceToGoalSq = Math::getDistanceSquared(newX, newY, targetX, targetY);
        float speedSq = movementComp->speed * movementComp->speed * deltaTime * deltaTime;
        
        // std::cout << "Speed: " << movementComp->speed << "; ";
        // std::cout << "newX: " << newX << "; newY: " << newY << "; x: " << positionComp->x << "; y: " << positionComp->y << "; ";
        // std::cout << "vX: " << movementComp->vx << "; vY: " << movementComp->vy << "; targetX: " << targetX << "; targetY: " << targetY << "; ";
        // std::cout << "dist: " << distanceToGoalSq << "; speedSq: " << speedSq << "\n";
        if (distanceToGoalSq <= speedSq) {
            setNewPosition(positionComp, targetX, targetY);
            movementComp->setVelocityToZero();
            
            if (movementComp->isGroundBased) {
                auto dFinalGoalSq = Math::getDistanceSquared(movementComp->goalX, movementComp->goalY, targetX, targetY);
                // std::cout << "Goal: " << movementComp->goalX << "; " << movementComp->goalY << "\n";
                // std::cout << "Final distance: " << dFinalGoalSq << "\n";
                if(dFinalGoalSq > 263.0) {
                    updateTempGoal(tilesComp, comp);
                    // std::cout << "movementComp.x: " << movementComp->tempGoalX << "; y: " << movementComp->tempGoalY << "\n";
                    movementComp->setVelocityTowardsGoal(positionComp->x, positionComp->y);
                } else {
                    movementComp->clearGoal();
                }
            } else {
                movementComp->clearGoal();
            }
        } else {
            setNewPosition(positionComp, newX, newY);
        }
    }

    void Game::updatePosition(float deltaTime, TilesComponent* tilesComp, Entity* entity) {
        auto pos = entity->getComponent<PositionComponent >();
        auto mov = entity->getComponent<MovementComponent >();
        auto col = entity->getComponent<CollisionComponent>();
        assert(pos); assert(mov); assert(col);
        updatePosition(deltaTime, tilesComp, _EntPosMovCol(nullptr, pos, mov, col));
    }

    void Game::updatePositionDirected(float deltaTime, TilesComponent* tilesComp, _EntPosMovCol comp) {
        auto entity       = std::get<0>(comp);
        auto positionComp = std::get<1>(comp);
        auto movementComp = std::get<2>(comp);
        assert(entity);
        assert(positionComp);
        assert(movementComp);
        auto projectileComp = entity->getComponent<ProjectileComponent>();
        if (projectileComp) {
            if (projectileComp->homing) {
                auto other = entityManager->getEntity(projectileComp->target);
                if (other) {
                    auto otherPosComp = other->getComponent<PositionComponent>();
                    if (otherPosComp) {
                        movementComp->setVelocityTowardsTarget(positionComp->x, positionComp->y, otherPosComp->x, otherPosComp->y);
                    }
                }
            }
        }
        float newX = positionComp->x + movementComp->vx * deltaTime;
        float newY = positionComp->y + movementComp->vy * deltaTime;
        setNewPosition(positionComp, newX, newY);
    }

    void Game::passPositionToChildren(Entity* entity, PositionComponent* positionComponent) {
        if (!entity) return;
        if (!positionComponent) return;

        auto children = entityManager->getChildren(entity->id);
        for (auto child : children) {
            auto posCompChild = child->getComponent<PositionComponent>();
            if (!posCompChild) continue;
            if (posCompChild->followParent) {
                auto center = getCenterOfEntity(entity, positionComponent);
                posCompChild->x = posCompChild->xRel + center.x;
                posCompChild->y = posCompChild->yRel + center.y;
            }
            passPositionToChildren(child, posCompChild);
        }
    }

    void Game::passPositionToChildren(Entity* entity) {
        if (!entity) return;
        auto posComp = entity->getComponent<PositionComponent>();
        passPositionToChildren(entity, posComp);
    }

    void Game::passPositionToChildren(EntityID entityID) {
        passPositionToChildren(entityManager->getEntity(entityID));
    }

    void Game::setNewPositionByDifference(PositionComponent* pos, float dx, float dy) {
        assert(pos);
        pos->x += dx; pos->y += dy;
    }

    void Game::setNewPosition(PositionComponent* pos, float x, float y) {
        assert(pos);
        pos->x = x; pos->y = y;
    }

    void Game::setNewCenteredPosition(Entity* entity, PositionComponent* pos, float x, float y) {
        auto center = getCenterOfEntity(entity, pos);
        float dx = x - center.x;
        float dy = y - center.y;
        setNewPositionByDifference(pos, dx, dy);
    }

    void Game::setNewCenteredPosition(Entity* entity, float x, float y) {
        assert(entity);
        auto pos = entity->getComponent<PositionComponent>();
        if (!pos) return;
        setNewCenteredPosition(entity, pos, x, y);
    }

}

#endif
