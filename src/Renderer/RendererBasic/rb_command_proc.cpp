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
        
        auto buildComp = asset->getComponent<BuildCommandComponent>();
        if (buildComp) {
            commandMode = RCommandMode::BUILD;
        }
        
        // Logger::getInstance().addMessage(MessageType::Error, "Unknown type: &1", comComp->commandType);
    }

    void RendererBasic::processCommandMessage(Entity* asset, std::string msg, const json &j) {
        Logger::getInstance().addMessage(MessageType::Info, msg);
    }

    bool RendererBasic::addVirtualEntityData(json &j) {
        if (!virtualEntity) return false;
        auto pos = virtualEntity->getComponent<PositionComponent>();
        auto col = virtualEntity->getComponent<CollisionComponent>();
        if (!pos || !col) return false;

        auto& bb = col->boundingBox;
        auto position = getMousePosWorld();
        pos->x = position.x - bb.w / 2;
        pos->y = position.y - bb.h / 2;
        j[JCOM_X] = pos->x;
        j[JCOM_Y] = pos->y;
        auto parentID = mouseToMap();
        if (parentID != NO_ENTITY) j[JCOM_PARENT] = parentID;
        return true;
    }

}
