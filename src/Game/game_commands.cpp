#include <Game/game.hpp>

#pragma once


namespace SupDef {

    void Game::updateCommands() {
        processDirectCommands();
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

        auto j = getFeedbackFromCheck(comProcessor->getData());
        globalDispatcher->dispatch<CommandToRenderEvent>(comProcessor->getCurrentCommand(), j, virtualEntity.get());
    }

    void Game::processDirectCommands() {
        assert(comProcessor);
        auto& directs = comProcessor->getDirects();
        for (auto& d : directs) {
            auto success = checkRequirements(d.commandID, d.data, CommandStatus::CONFIRMED, false);
            if (success) {
                std::stringstream ss;
                ss << "Direct command " << d.commandID << " successful!\n";
                auto j = getFeedbackFromCheck(d.data, ss.str());
                globalDispatcher->dispatch<CommandToRenderEvent>(d.commandID, j);
            }
        }
        directs.clear();
    }

    json Game::getFeedbackFromCheck(json& input) {
        return getFeedbackFromCheck(input, "");
    }

    json Game::getFeedbackFromCheck(json& input, std::string msg) {
        json j;
        if (!msg.empty()) {
            j[JCOM_MESSAGE] = msg;
        }
        // Add additional feedback...
        return j;
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

}
