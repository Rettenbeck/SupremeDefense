#include <Game/game.hpp>

#pragma once


namespace SupDef {

    void Game::processTechs() {
        removeObsoleteGiftedTechs();

        auto techs = entityManager->getEntitiesWithComponents<TechComponent>();
        assignTechs(techs);
        distributeTechAssignment(techs);

        for (auto& [entityID, techAssignments] : assigneeToTechs) {
            processTechsForEntity(entityID, techAssignments);
        }
    }

    void Game::processTechsForEntity(EntityID entityID, TechAssignments& techAssignments) {
        auto entity = entityManager->getEntity(entityID);
        if (!entity) return;

        for (auto [techID, persistence] : techAssignments) {
            auto tech = entityManager->getEntity(techID);
            if (!tech) continue;

            auto giftComp = tech->getComponent<GiftTechComponent>();
            if (giftComp) {
                if (persistence == TechPersistence::Gained) {
                    createGiftedTech(entity, tech, giftComp);
                } else {
                    removeGiftedTech(entity, tech, giftComp);
                }
            }
        }
    }

    void Game::assignTechs(_EntTechs& techs) {
        auto vec2str = [](EntityIDs& ids) {
            std::stringstream ss;
            for (auto id : ids) ss << id << "  ";
            return ss.str();
        };

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
        if (!log_assert(owner)) return;
        auto parent = entityManager->getParent(owner->id);
        auto children = entityManager->getChildren(owner->id);
        auto ownComp = owner->getComponent<PlayerOwnershipComponent>();

        if (tech->applyToOwner) {
            tech->addAssignee(owner->id);
        }

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
            auto allUnits = entityManager->getEntitiesWithComponents<PlayerOwnershipComponent>();
            for (auto [child, childOwnerComp] : allUnits) {
                tech->addAssignee(child->id);
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
                    if (otherID != owner->id) tech->addAssignee(otherID);
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
            if (!log_assert(assignee)) continue;
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

    void Game::distributeTechAssignment(_EntTechs& techList) {
        techToAssignees.clear();
        assigneeToTechs.clear();
        for (auto [tech, techComp] : techList) {
            for (auto assignee : techComp->gained) {
                techToAssignees[tech->id].emplace_back(assignee, TechPersistence::Gained);
                assigneeToTechs[assignee].emplace_back(tech->id, TechPersistence::Gained);
            }
            for (auto assignee : techComp->kept) {
                techToAssignees[tech->id].emplace_back(assignee, TechPersistence::Kept);
                assigneeToTechs[assignee].emplace_back(tech->id, TechPersistence::Kept);
            }
            for (auto assignee : techComp->lost) {
                techToAssignees[tech->id].emplace_back(assignee, TechPersistence::Lost);
                assigneeToTechs[assignee].emplace_back(tech->id, TechPersistence::Lost);
            }
        }
    }
    
    void Game::createGiftedTech(Entity* gifted, Entity* gifter, GiftTechComponent* giftComp) {
        if (!log_assert(gifted  )) return;
        if (!log_assert(gifter  )) return;
        if (!log_assert(giftComp)) return;

        for (auto techToCreate : giftComp->techsToCreate) {
            auto createdTech = createEntityFromAsset(techToCreate, gifted->id);
            assert(createdTech);
            auto createdTechComp = createdTech->getComponent<TechComponent>();
            assert(createdTechComp);
            giftComp->addCreatedTech(gifted->id, createdTech->id);
            createdTechComp->createdBy = gifter->id;
        }
    }

    void Game::removeGiftedTech(Entity* gifted, Entity* gifter, GiftTechComponent* giftComp) {
        if (!log_assert(gifted  )) return;
        if (!log_assert(gifter  )) return;
        if (!log_assert(giftComp)) return;

        if (giftComp->createdTechs.count(gifted->id)) {
            for(auto techID : giftComp->createdTechs[gifted->id]) {
                entityManager->removeEntity(techID);
                giftComp->removeCreatedTech(gifted->id, techID);
            }
        }
    }

    void Game::removeObsoleteGiftedTechs() {
        auto techs = entityManager->getEntitiesWithComponents<TechComponent>();
        for (auto [tech, techComp] : techs) {
            if (techComp->createdBy != NO_ENTITY) {
                auto gifter = entityManager->getEntity(techComp->createdBy);
                if (!gifter) {
                    entityManager->removeEntity(tech->id);
                }
            }
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