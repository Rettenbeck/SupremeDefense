#ifndef GAME_PLAYER
#define GAME_PLAYER

#include <Game/game.hpp>


namespace SupDef {

    Entity* Game::getPlayer(Entity* entity) {
        if (!entity) return nullptr;
        auto playerComp = entity->getComponent<PlayerComponent>();
        if (playerComp) return entity;
        auto ownerComp = entity->getComponent<PlayerOwnershipComponent>();
        if (ownerComp) return entityManager->getEntity(ownerComp->ownerID);
        auto parent = entityManager->getParent(entity->id);
        if (parent) return getPlayer(parent);
        return nullptr;
    }

    Entity* Game::getPlayer(EntityID entityID) {
        return getPlayer(entityManager->getEntity(entityID));
    }

    Relation Game::getRelation(Entity* entity1, Entity* entity2) {
        auto player1 = getPlayer(entity1);
        auto player2 = getPlayer(entity2);
        if (!player1 || !player2) return Relation::Undefined;
        if (player1->id == player2->id) return Relation::Same;
        auto playerComp1 = player1->getComponent<PlayerComponent>();
        auto playerComp2 = player2->getComponent<PlayerComponent>();
        assert(playerComp1); assert(playerComp2);
        if (playerComp1->isNeutral || playerComp2->isNeutral) return Relation::Neutral;
        if (playerComp1->teamID == playerComp2->teamID && playerComp1->teamID != NO_ENTITY) return Relation::Allied;
        return Relation::Opposing;
    }

    Relation Game::getRelation(EntityID entityID1, EntityID entityID2) {
        return getRelation(entityManager->getEntity(entityID1), entityManager->getEntity(entityID2));
    }

    void Game::retrieveChildrenRecursive(EntityID entityID, PEntities& list) {
        list.push_back(entityManager->getEntity(entityID));
        auto children = entityManager->getChildren(entityID);
        for (auto child : children) {
            retrieveChildrenRecursive(child->id, list);
        }
    }

    PEntities Game::retrieveChildrenRecursive(EntityID entityID) {
        PEntities list;
        retrieveChildrenRecursive(entityID, list);
        return list;
    }

}

#endif
