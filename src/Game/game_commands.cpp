#include <Game/game.hpp>

#pragma once


namespace SupDef {

    void Game::updateCommands() {
        assert(comProcessor);
        auto command = comProcessor->getCurrentCommand();
        auto status = comProcessor->getCommandStatus();
        Entity* asset  = nullptr;
        std::stringstream ss;

        if (status == CommandStatus::NONE) {
            comProcessor->reset();
            globalDispatcher->dispatch<CommandToRenderEvent>(NO_COMMAND, json());
            return;
        }
        if (command != NO_COMMAND) {
            asset = getAssetFromCommand(command, comProcessor->getData());
            assert(asset);
        }
        auto success = checkRequirements(command, comProcessor->getData(), status, false);

        switch (status) {
            case CommandStatus::RECEIVED:
                ss << "Command " << command << " ";
                if (success) ss << "accepted!";
                else ss << "failed!";
                ss << "\n";
                comProcessor->setCommandStatus(CommandStatus::ONGOING);
                if (success) {
                    handleStartCommand(asset);
                } else {
                    comProcessor->reset();
                }
                break;
            case CommandStatus::ONGOING:
                handleUpdateCommand(asset);
                break;
            case CommandStatus::CONFIRMED:
                ss << "Command " << command << " ";
                if (!success) ss << "not";
                ss << " successful!\n";
                if (success) handleConfirmCommand(asset);
                comProcessor->reset();
                break;
            case CommandStatus::CANCELLED:
                ss << "Command " << command << " cancelled!\n";
                comProcessor->reset();
                break;
            default:
                assert(false);
        }

        json j;
        std::string msg = ss.str();
        if (!msg.empty()) {
            j[JCOM_MESSAGE] = msg;
        }
        globalDispatcher->dispatch<CommandToRenderEvent>(comProcessor->getCurrentCommand(), j, virtualEntity.get());
    }

    void Game::handleStartCommand(Entity* command) {
        assert(command);
        auto comComp = command->getComponent<CommandComponent>();
        assert(comComp);

        auto buildComp = command->getComponent<BuildCommandComponent>();
        if (buildComp) {
            createVirtualEntityFromAsset(buildComp->toBuild);
            assert(virtualEntity);
        }
    }

    void Game::handleUpdateCommand(Entity* command) {
        assert(command);
        //
    }

    void Game::handleConfirmCommand(Entity* command) {
        assert(command);
        auto action = std::make_shared<Action>(command->assetID, thisPlayer, comProcessor->getData());
        globalDispatcher->dispatch<SupDef::ActionCreatedEvent>(action);
        virtualEntity = nullptr;
    }

    Entity* Game::getAssetFromCommand(CommandID commandID, json &data) {
        if (commandID == NO_COMMAND) return nullptr;
        auto asset = assetManager->getAsset(commandID);
        if (!asset) {
            Logger::getInstance().addMessage(MessageType::Error, "Command &1 not found!", commandID);
            return nullptr;
        }
        auto comComp = asset->getComponent<CommandComponent>();
        if (!comComp) {
            Logger::getInstance().addMessage(MessageType::Error, "&1 not a command!", commandID);
            return nullptr;
        }
        if (comComp->isUnique) {
            if (!data.contains(JCOM_UNIQUE)) {
                Logger::getInstance().addMessage(MessageType::Error,
                    "&1 marked as unique but does not contain command!", commandID);
                uniqueCommand = nullptr;
                return nullptr;
            }
            json j = data[JCOM_UNIQUE];
            uniqueCommand = std::make_unique<Entity>(NO_ENTITY);
            uniqueCommand->from_json(j);
            comComp = uniqueCommand->getComponent<CommandComponent>();
            if (!comComp) {
                Logger::getInstance().addMessage(MessageType::Error, "Inside asset not a command!", commandID);
                return nullptr;
            }
            return uniqueCommand.get();
        }
        return asset;
    }

    bool Game::checkRequirements(CommandID commandID, json &data, CommandStatus status, bool onAction) {
        assert(assetManager);
        auto asset = getAssetFromCommand(commandID, data);
        assert(asset);
        auto reqComp = asset->getComponent<RequirementComponent>();
        if (!reqComp) return (thisPlayer != NO_ENTITY);
        return checkRequirements(reqComp, status, onAction);
    }

    bool Game::checkRequirements(RequirementComponent* reqComp, CommandStatus status, bool onAction) {
        auto player = entityManager->getEntity(thisPlayer);
        if (!player) return false;
        
        if (!checkResourceReq(player, reqComp, status, onAction)) return false;
        // Additional checks...
        return true;
    }

    bool Game::checkResourceReq(Entity* player, RequirementComponent* reqComp, CommandStatus status, bool onAction) {
        assert(player);
        assert(reqComp);
        if (!reqComp->resources.empty()) {
            auto resComp = player->getComponent<ResourceComponent>();
            if (!resComp) return false;
            for(const auto& resourceReq : reqComp->resources) {
                if (!resourceReq) continue;
                auto it = resComp->resources.find(resourceReq->resourceID);
                if (it == resComp->resources.end()) return false;
                auto& resourcePlayer = it->second;
                if (resourcePlayer->amount < resourceReq->amount) return false;
                if (onAction) {
                    resourcePlayer->amount -= resourceReq->amount;
                }
            }
        }
        return true;
    }

}
