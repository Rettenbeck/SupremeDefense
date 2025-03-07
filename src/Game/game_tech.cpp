#include <Game/game.hpp>

#pragma once


namespace SupDef {

    void Game::processTechs() {
        auto techs = entityManager->getEntitiesWithComponents<TechComponent>();
        std::unordered_map<EntityID, EntityIDs> techToAssignees;
        std::unordered_map<EntityID, EntityIDs> assigneeToTechs;

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

}
