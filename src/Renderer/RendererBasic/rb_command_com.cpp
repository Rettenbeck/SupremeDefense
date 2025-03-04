#include <Renderer/renderer_basic.hpp>

#pragma once


namespace SupDef {

    void RendererBasic::trigger() {
        std::cout << "Command triggered!\n";
        globalDispatcher->dispatch<StartCommandEvent>("test_command");
    }

    void RendererBasic::subscribeToEvents() {
        globalDispatcher->subscribe<CommandToRenderEvent>([this](const SupDef::Events& events) {
            for (const auto& event : events) {
                const auto& typedEvent = static_cast<const SupDef::CommandToRenderEvent&>(*event);
                onCommandToRender(typedEvent);
            }
        });
        
    }

    void RendererBasic::onCommandToRender(const CommandToRenderEvent& event) {
        if (!game) return;
        if (!game->getAssetManager()) return;
        currentCommand = event.commandID;
        auto asset = game->getAssetManager()->getAsset(event.commandID);
        processCommand(asset, event.data);
    }

    void RendererBasic::updateCommand() {
        if (commandMode == RCommandMode::NONE) return;
        json j;
        auto pos = getMousePosWorld();
        j["x"] = pos.x;
        j["y"] = pos.y;
        globalDispatcher->dispatch<UpdateCommandEvent>(j);
    }

    void RendererBasic::resetCommand() {
        currentCommand = NO_COMMAND;
        commandMode = RCommandMode::NONE;
    }

}
