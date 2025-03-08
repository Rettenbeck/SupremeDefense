#include <Game/game.hpp>

#pragma once


namespace SupDef {

    void Game::updateTempGoalMass(TilesComponent* tilesComp, _EntPosMovCols& comps) {
        for(auto& comp : comps) {
            updateTempGoal(tilesComp, comp);
        }
    }

    void Game::updateTempGoal(TilesComponent* tilesComp, _EntPosMovCol comp) {
        auto positionComp = std::get<1>(comp);
        auto movementComp = std::get<2>(comp);
        auto collisionComp = std::get<3>(comp);
        assert(tilesComp); assert(positionComp); assert(movementComp); assert(collisionComp);

        if(!movementComp->hasGoal) {
            movementComp->setVelocityToZero();
            return;
        }

        auto path = pathFinder->findPathForUnit(tilesComp, positionComp->x, positionComp->y,
                                                movementComp->goalX, movementComp->goalY, collisionComp->dummyRadius);
        if (path.size() <= 1) {
            movementComp->clearGoal();
            movementComp->setVelocityToZero();
        } else {
            auto& p = path[1];
            movementComp->tempGoalX = p.x;
            movementComp->tempGoalY = p.y;
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
        auto positionComp = std::get<1>(comp);
        auto movementComp = std::get<2>(comp);
        auto collisionComp = std::get<3>(comp);
        assert(tilesComp); assert(positionComp); assert(movementComp); assert(collisionComp);

        if(!movementComp->hasGoal) {
            movementComp->setVelocityToZero();
            return;
        }
    
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
            positionComp->x = targetX;
            positionComp->y = targetY;
            movementComp->setVelocityToZero();
            
            if (movementComp->isGroundBased) {
                auto dFinalGoalSq = Math::getDistanceSquared(movementComp->goalX, movementComp->goalY, targetX, targetY);
                // std::cout << "Final distance: " << dFinalGoalSq << "\n";
                if(dFinalGoalSq > 3.0) {
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
            positionComp->x = newX;
            positionComp->y = newY;
        }
    }

    void Game::updatePosition(float deltaTime, TilesComponent* tilesComp, Entity* entity) {
        auto pos = entity->getComponent<PositionComponent>();
        auto mov = entity->getComponent<MovementComponent>();
        auto col = entity->getComponent<CollisionComponent>();
        assert(pos); assert(mov); assert(col);
        updatePosition(deltaTime, tilesComp, _EntPosMovCol(nullptr, pos, mov, col));
    }

    void Game::passPositionToChildren(Entity* entity, PositionComponent* positionComponent) {
        if (!entity) return;
        if (!positionComponent) return;

        auto children = entityManager->getChildren(entity->id);
        for (auto child : children) {
            auto posCompChild = child->getComponent<PositionComponent>();
            if (!posCompChild) continue;
            posCompChild->xAbs = posCompChild->x + positionComponent->xAbs;
            posCompChild->yAbs = posCompChild->y + positionComponent->yAbs;
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

}
