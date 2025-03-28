#include <Game/game.hpp>

#pragma once


namespace SupDef {

    void Game::processTechs() {
        techToAssignees.clear();
        assigneeToTechs.clear();
        auto techs = entityManager->getEntitiesWithComponents<TechComponent>();
        assignTechs(techs);

        for (auto [tech, techComp] : techs) {
            for (auto assignee : techComp->assignees) {
                techToAssignees[tech->id].push_back(assignee);
                assigneeToTechs[assignee].push_back(tech->id);
            }
        }

        for (auto& [entityID, techIDs] : assigneeToTechs) {
            processTechsForEntity(entityID, techIDs);
        }
    }

    void Game::processTechsForEntity(EntityID entityID, EntityIDs& techIDs) {
        //
    }

    void Game::assignTechs(_EntTechs& techs) {
        for (auto [entity, tech] : techs) {
            auto previous_assignees = tech->assignees;
            tech->assignees.clear();
            assignTech(entity, tech);
            filterTech(entity, tech);
            filterTechByComponents(entity, tech);
            assignTechByList(entity, tech);

            auto [kept, lost, gained] = compareVectors(previous_assignees, tech->assignees);
            tech->gained = gained;
            tech->kept = kept;
            tech->lost = lost;
        }

    }

    void Game::assignTech(Entity* entity, TechComponent* tech) {
        auto owner = entityManager->getParent(entity->id);
        assert(owner);
        auto parent = entityManager->getParent(owner->id);
        auto children = entityManager->getChildren(owner->id);
        auto ownComp = owner->getComponent<PlayerOwnershipComponent>();

        if (tech->applyToParent) {
            if (parent) {
                tech->addAssignee(parent->id);
            }
        }

        if (tech->applyToChildren) {
            for (auto child : children) {
                tech->addAssignee(child->id);
            }
        }

        if (tech->applyToAll) {
            if (ownComp) {
                auto allUnitsOfPlayer = retrieveChildrenRecursive(ownComp->ownerID);
                for (auto child : allUnitsOfPlayer) {
                    tech->addAssignee(child->id);
                }
            }
        }

        if (tech->applyToWithinInfluence) {
            auto mapID = getMapOfEntity(entity->id);
            auto collisionGroup = buildCollisionGroup(CG_INFLUENCE, mapID);
            auto collisions = collisionTracker->retrieve(entity->id);
            for (auto collision : collisions) {
                if (collision->collisionGroup == collisionGroup) {
                    auto otherID = collision->entityA;
                    if (otherID == entity->id) otherID = collision->entityB;
                    tech->addAssignee(otherID);
                }
            }
        }
    }

    void Game::filterTech(Entity* entity, TechComponent* tech) {
        if (tech->affectsAll) return;
        if (!tech->affectsSamePlayer && !tech->affectsSameTeam && !tech->affectsEnemy) {
            tech->assignees.clear();
            return;
        }

        EntityIDs filtered;
        for (auto assignee : tech->assignees) {
            auto relation = getRelation(entity->id, assignee);
            switch (relation) {
                case Relation::Same:
                    if (tech->affectsSamePlayer) filtered.push_back(assignee);
                    break;
                case Relation::Allied:
                    if (tech->affectsSameTeam) filtered.push_back(assignee);
                    break;
                case Relation::Opposing:
                    if (tech->affectsEnemy) filtered.push_back(assignee);
                    break;
                default:
                    break;
            }
        }
        tech->assignees = filtered;
    }

    void Game::filterTechByComponents(Entity* entity, TechComponent* tech) {
        if (!tech->filterByComponents) return;
        EntityIDs filtered;
        for (auto id : tech->assignees) {
            auto assignee = entityManager->getEntity(id);
            assert(assignee);
            bool result = true;
            for (auto comp : tech->requiredComponents) {
                if (!assignee->hasComponentByName(comp)) {
                    result = false;
                    break;
                }
            }
            if (result) { filtered.push_back(id); }
        }
        tech->assignees = filtered;
    }

    void Game::assignTechByList(Entity* entity, TechComponent* tech) {
        for (auto id : tech->whitelist) {
            tech->addAssignee(id);
        }
        for (auto id : tech->blacklist) {
            tech->removeAssignee(id);
        }
    }

    EntityIDsTriple Game::compareVectors(const EntityIDs& vec1, const EntityIDs& vec2) {
        std::unordered_set<EntityID> set1(vec1.begin(), vec1.end());
        std::unordered_set<EntityID> set2(vec2.begin(), vec2.end());
        EntityIDs in_both, only_in_1, only_in_2;

        for (int val : set1) {  // Entries from vec1
            if (set2.count(val)) in_both.push_back(val);
            else only_in_1.push_back(val);
        }
        for (int val : set2) {  // Entries only in vec2
            if (!set1.count(val)) only_in_2.push_back(val);
        }

        return {in_both, only_in_1, only_in_2};
    }

    Entity* Game::getCommandFromActiveTech(Entity* tech) {
        assert(tech);
        auto activeTechComp = tech->getComponent<ActiveTechComponent>();
        return getCommandFromActiveTech(activeTechComp);
    }

    Entity* Game::getCommandFromActiveTech(ActiveTechComponent* activeTechComp) {
        assert(activeTechComp);
        auto command = assetManager->getAsset(activeTechComp->commandID);
        if (command) {
            if (command->hasComponent<CommandComponent>()) return command;
        }
        return nullptr;
    }

}



        // auto movers = entityManager->getEntitiesWithComponents<
        //     PositionComponent, MovementComponent, CollisionComponent, SelectableComponent>();
        
        // auto players = entityManager->getEntitiesWithComponents<PlayerComponent>();
        // MoveCommandComponent* mcComp = nullptr;
        // Entity* moveTech = nullptr;
        // for (auto [entity, player] : players) {
        //     auto children = entityManager->getChildren(entity->id);
        //     for (auto child : children) {
        //         auto command = getCommandFromActiveTech(child);
        //         if (command) {
        //             mcComp = command->getComponent<MoveCommandComponent>();
        //             if (mcComp) moveTech = child;
        //         }
        //         if (mcComp) break;
        //     }
        //     if (mcComp) break;
        // }

        // if (mcComp) {
        //     assert(moveTech);
        //     auto techComp = moveTech->getComponent<TechComponent>();
        //     assert(techComp);
        //     for(auto [ent, pos, mov, col, sel] : movers) {
        //         techComp->addAssignee(ent->id);
        //     }
        // }

        // auto actives = entityManager->getEntitiesWithComponents<TechComponent, ActiveTechComponent>();
        // for(auto [ent, tech, active] : actives) {
        //     auto parent = entityManager->getParent(ent->id);
        //     if (!parent) continue;
        //     tech->addAssignee(parent->id);
        // }