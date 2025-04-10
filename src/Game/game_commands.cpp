// #pragma once
#ifndef GAME_COMMANDS
#define GAME_COMMANDS

#include <Game/game.hpp>


namespace SupDef {

    void Game::handleTriggerCommand(const TriggerCommandEvent& event) {
        if (event.cancel) {
            commandTracker->reset();
            globalDispatcher->dispatch<CommandToRenderEvent>(NO_COMMAND, NO_ENTITY, NO_ENTITY, json());
            virtualEntity = nullptr;
            return;
        }
        json data = event.data;

        auto [valid, command, active] = isTechProperCommand(event.entityID, event.techID);
        if (!valid) return;

        auto success = checkRequirements(command->assetID, data, false);
        if (!success) {
            auto j = getFeedbackFromCheck(data, "Requirements not met");
            globalDispatcher->dispatch<CommandToRenderEvent>(active->commandID, event.entityID, event.techID, j);
            return;
        }

        auto complete = isCommandComplete(command, data);
        if (complete) {
            commandTracker->reset();
            handleCompleteCommand(event.entityID, event.techID, command->assetID, data);
        } else {
            commandTracker->set(event.entityID, event.techID, active->commandID);
            handleIncompleteCommand(command, event.entityID, event.techID, data);
        }
    }

    void Game::handleUpdateCommand(const UpdateCommandEvent& event) {
        //
    }

    void Game::handleCompleteCommand(EntityID entityID, EntityID techID, CommandID commandID, json data) {
        std::stringstream ss;
        ss << "Command " << commandID << " successful!\n";
        auto j = getFeedbackFromCheck(data);
        globalDispatcher->dispatch<CommandToRenderEvent>(commandID, entityID, techID, j);
        auto action = std::make_shared<Action>(commandID, entityID, thisPlayer, j);
        globalDispatcher->dispatch<SupDef::ActionCreatedEvent>(action);
        virtualEntity = nullptr;
    }

    void Game::handleIncompleteCommand(Entity* command, EntityID entityID, EntityID techID, json j) {
        auto buildComp = command->getComponent<BuildCommandComponent>();
        if (buildComp) {
            createVirtualEntityFromAsset(buildComp->toBuild);
            assert(virtualEntity);
            globalDispatcher->dispatch<CommandToRenderEvent>(command->assetID, entityID, techID, j);
        }
        auto moveComp = command->getComponent<MoveCommandComponent>();
        if (moveComp) {
            globalDispatcher->dispatch<CommandToRenderEvent>(command->assetID, entityID, techID, j);
        }
    }

    bool Game::isCommandComplete(Entity* command, json j) {
        auto buildComp = command->getComponent<BuildCommandComponent>();
        if (buildComp) {
            if (!j.contains(JCOM_X) || !j.contains(JCOM_Y)) {
                return false;
            }
        }
        auto moveComp = command->getComponent<MoveCommandComponent>();
        if (moveComp) {
            if (!j.contains(JCOM_X) || !j.contains(JCOM_Y)) {
                return false;
            }
        }
        return true;
    }

    std::tuple<bool, Entity*, ActiveTechComponent*> Game::isTechProperCommand(EntityID entityID, EntityID techID) {
        auto tech = entityManager->getEntity(techID);
        if (!tech) {
            LOG_ERROR("Tech &1 does not exist!", techID)
            return {false, nullptr, nullptr};
        }
        auto techComp = tech->getComponent<TechComponent>();
        auto activeComp = tech->getComponent<ActiveTechComponent>();
        if (!techComp || !activeComp) {
            LOG_ERROR("Entity &1 not a tech!", techID)
            return {false, nullptr, nullptr};
        }
        auto it = std::find(techComp->assignees.begin(), techComp->assignees.end(), entityID);
        if (it == techComp->assignees.end()) {
            LOG_ERROR("Entity &1 not asssigned to tech!", entityID)
            return {false, nullptr, nullptr};
        }
        json j;
        auto command = getAssetFromCommand(activeComp->commandID, j);
        if (!command) {
            LOG_ERROR("Command &1 does not exist!", activeComp->commandID)
            return {false, nullptr, nullptr};
        }
        return {true, command, activeComp};
    }

    Entity* Game::getAssetFromCommand(CommandID commandID, json &data) {
        if (commandID == NO_COMMAND) return nullptr;
        auto asset = assetManager->getAsset(commandID);
        if (!asset) {
            LOG_ERROR("Command &1 not found!", commandID)
            return nullptr;
        }
        auto comComp = asset->getComponent<CommandComponent>();
        if (!comComp) {
            LOG_ERROR("&1 not a command!", commandID)
            return nullptr;
        }
        if (comComp->isUnique) {
            if (!data.contains(JCOM_UNIQUE)) {
                LOG_ERROR("&1 marked as unique but does not contain command!", commandID)
                uniqueCommand = nullptr;
                return nullptr;
            }
            json j = data[JCOM_UNIQUE];
            uniqueCommand = std::make_unique<Entity>(NO_ENTITY);
            uniqueCommand->from_json(j);
            comComp = uniqueCommand->getComponent<CommandComponent>();
            if (!comComp) {
                LOG_ERROR("Inside asset not a command!")
                return nullptr;
            }
            return uniqueCommand.get();
        }
        return asset;
    }

    json Game::getFeedbackFromCheck(json& input) {
        return getFeedbackFromCheck(input, "");
    }

    json Game::getFeedbackFromCheck(json& input, std::string msg) {
        json j = input;
        if (!msg.empty()) {
            j[JCOM_MESSAGE] = msg;
        }
        // Add additional feedback...
        return j;
    }

}

#endif
