#include <Game/game.hpp>

#pragma once


namespace SupDef {

    void Game::updateCommands() {
        assert(comProcessor);
        auto command = comProcessor->getCurrentCommand();
        auto status = comProcessor->getCommandStatus();
        std::stringstream ss;

        if (status == CommandStatus::NONE) {
            comProcessor->reset();
            globalDispatcher->dispatch<CommandToRenderEvent>(NO_COMMAND, json());
            return;
        }
        auto success = checkRequirements(command, status);

        switch (status) {
            case CommandStatus::RECEIVED:
                // globalDispatcher->dispatch<StartCommandReceivedEvent>(command, success);
                ss << "Command received: " << command << "\n";
                comProcessor->setCommandStatus(CommandStatus::ONGOING);
                if (!success) { comProcessor->reset(); }
                break;
            case CommandStatus::ONGOING:
                // globalDispatcher->dispatch<UpdateCommandReceivedEvent>(json());
                break;
            case CommandStatus::CONFIRMED:
                // globalDispatcher->dispatch<ConfirmCommandReceivedEvent>(success, json());
                ss << "Command " << command << " ";
                if (!success) ss << "not";
                ss << " successful!\n";
                comProcessor->reset();
                break;
            case CommandStatus::CANCELLED:
                // globalDispatcher->dispatch<ConfirmCommandReceivedEvent>(success, json());
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
        globalDispatcher->dispatch<CommandToRenderEvent>(comProcessor->getCurrentCommand(), j);
    }

    bool Game::checkRequirements(RequirementComponent* reqComp, CommandStatus status) {
        auto player = entityManager->getEntity(thisPlayer);
        if (!player) return false;
        
        if (!checkResourceRequirements(player, reqComp, status)) return false;
        // Additional checks...
        return true;
    }

    bool Game::checkRequirements(CommandID commandID, CommandStatus status) {
        assert(assetManager);
        auto asset = assetManager->getAsset(commandID);
        assert(asset);
        auto reqComp = asset->getComponent<RequirementComponent>();
        if (!reqComp) return (thisPlayer != NO_ENTITY);
        return checkRequirements(reqComp, status);
    }

    bool Game::checkResourceRequirements(Entity* player, RequirementComponent* reqComp, CommandStatus status) {
        assert(player);
        //if(status == CommandStatus::ONGOING) return true;

        if (!reqComp->resources.empty()) {
            auto resComp = player->getComponent<ResourceComponent>();
            if (!resComp) return false;
            for(const auto& resourceReq : reqComp->resources) {
                if (!resourceReq) continue;
                auto it = resComp->resources.find(resourceReq->resourceID);
                if (it == resComp->resources.end()) return false;
                auto& resourcePlayer = it->second;
                if (resourcePlayer->amount < resourceReq->amount) return false;
                if (status == CommandStatus::CONFIRMED) {
                    resourcePlayer->amount -= resourceReq->amount;
                }
            }
        }
        return true;
    }

}
