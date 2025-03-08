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
            tech->assignees.clear();
        }
        
        auto movers = entityManager->getEntitiesWithComponents<
            PositionComponent, MovementComponent, CollisionComponent, SelectableComponent>();
        
        auto players = entityManager->getEntitiesWithComponents<PlayerComponent>();
        MoveCommandComponent* mcComp = nullptr;
        Entity* moveTech = nullptr;
        for (auto [entity, player] : players) {
            auto children = entityManager->getChildren(entity->id);
            for (auto child : children) {
                auto command = getCommandFromActiveTech(child);
                if (command) {
                    mcComp = command->getComponent<MoveCommandComponent>();
                    if (mcComp) moveTech = child;
                }
                if (mcComp) break;
            }
            if (mcComp) break;
        }

        if (mcComp) {
            assert(moveTech);
            auto techComp = moveTech->getComponent<TechComponent>();
            assert(techComp);
            for(auto [ent, pos, mov, col, sel] : movers) {
                techComp->addAssignee(ent->id);
            }
        }

        auto actives = entityManager->getEntitiesWithComponents<TechComponent, ActiveTechComponent>();
        for(auto [ent, tech, active] : actives) {
            auto parent = entityManager->getParent(ent->id);
            if (!parent) continue;
            tech->addAssignee(parent->id);
        }
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
