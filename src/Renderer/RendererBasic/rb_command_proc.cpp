#ifndef RENDERER_BASIC_PROC
#define RENDERER_BASIC_PROC

#include <Renderer/renderer_basic.hpp>


namespace SupDef {

    void RendererBasic::processCommand(Entity* asset, const json &j) {
        if (j.contains(JCOM_MESSAGE)) processCommandMessage(asset, j.at(JCOM_MESSAGE).get<std::string>(), j);
        if (currentCommand == NO_COMMAND) {
            resetCommand();
            return;
        }
        if (!asset) {
            resetCommand();
            LOG_ERROR("Unknown command: &1", currentCommand)
            return;
        }
        if (!game->getCommandTracker()->ongoing) {
            commandMode = RCommandMode::NONE;
            return;
        }

        auto comComp = asset->getComponent<CommandComponent>();
        assert(comComp);
        
        auto moveComp = asset->getComponent<MoveCommandComponent>();
        if (moveComp) {
            commandMode = RCommandMode::MOVE;
        }

        auto buildComp = asset->getComponent<BuildCommandComponent>();
        if (buildComp) {
            commandMode = RCommandMode::BUILD;
        }
    }

    void RendererBasic::processCommandMessage(Entity* asset, std::string msg, const json &j) {
        LOG(Info, msg)
    }

    bool RendererBasic::addPositionData(json &j) {
        if (!game) return false;
        auto position = getMousePosWorld();
        j[JCOM_X] = position.x;
        j[JCOM_Y] = position.y;
        auto mapID = mouseToMap();
        if (mapID != NO_ENTITY) j[JCOM_MAP] = mapID;
        return true;
    }

    bool RendererBasic::addVirtualEntityData(json &j) {
        if (!virtualEntity) return false;
        auto pos = virtualEntity->getComponent<PositionComponent>();
        auto col = virtualEntity->getComponent<CollisionComponent>();
        if (!pos || !col) return false;

        auto& bb = col->boundingBox;
        auto position = getMousePosWorld();
        pos->x = position.x;
        pos->y = position.y;
        j[JCOM_X] = pos->x;
        j[JCOM_Y] = pos->y;
        auto mapID = mouseToMap();
        if (mapID != NO_ENTITY) j[JCOM_MAP] = mapID;
        return true;
    }

}

#endif
