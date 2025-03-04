#include <Renderer/renderer_basic.hpp>

#pragma once


namespace SupDef {

    void RendererBasic::processCommand(Entity* asset, const json &j) {
        if (j.contains(JCOM_MESSAGE)) processCommandMessage(asset, j.at(JCOM_MESSAGE).get<std::string>(), j);
        if (currentCommand == NO_COMMAND) {
            resetCommand();
            return;
        }
        if (!asset) {
            resetCommand();
            Logger::getInstance().addMessage(MessageType::Error, "Unknown command: &1", currentCommand);
            return;
        }

        auto comComp = asset->getComponent<CommandComponent>();
        assert(comComp);
        
        if (comComp->commandType == COM_TYPE_BUILD) {
            commandMode = RCommandMode::BUILD;
        } else {
            Logger::getInstance().addMessage(MessageType::Error, "Unknown type: &1", comComp->commandType);
        }
    }

    void RendererBasic::processCommandMessage(Entity* asset, std::string msg, const json &j) {
        Logger::getInstance().addMessage(MessageType::Info, msg);
    }

}
