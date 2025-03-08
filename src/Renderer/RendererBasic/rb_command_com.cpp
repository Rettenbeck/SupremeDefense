#include <Renderer/renderer_basic.hpp>

#pragma once


namespace SupDef {

    void RendererBasic::trigger(int i) {
        // std::cout << "Command " << i << " triggered!\n";
        // if(i == 1) globalDispatcher->dispatch<StartCommandEvent>("test_command");
        // if(i == 2) globalDispatcher->dispatch<StartCommandEvent>("test_command2");
        // if(i == 3) globalDispatcher->dispatch<DirectCommandEvent>("test_command3", true);
        // if(i == 4) globalDispatcher->dispatch<DirectCommandEvent>("test_command4", true);
        // if(i == 5) globalDispatcher->dispatch<DirectCommandEvent>("test_command5", true);
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
        virtualEntity = game->getVirtualEntity();
        json j = event.data;
        auto asset = game->getAssetFromCommand(event.commandID, j);
        processCommand(asset, event.data);
    }

    void RendererBasic::updateCommand() {
        json j;
        if (commandMode == RCommandMode::NONE) return;
        if (addVirtualEntityData(j)) globalDispatcher->queueEvent<UpdateCommandEvent>(j);
    }

    void RendererBasic::resetCommand() {
        currentCommand = NO_COMMAND;
        commandMode = RCommandMode::NONE;
    }

}
