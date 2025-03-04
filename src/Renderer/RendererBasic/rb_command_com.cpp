#include <Renderer/renderer_basic.hpp>

#pragma once


namespace SupDef {

    void RendererBasic::trigger() {
        std::cout << "Command triggered!\n";
        globalDispatcher->dispatch<StartCommandEvent>("test_command");
    }

    void RendererBasic::subscribeToEvents() {
        // globalDispatcher->subscribe<StartCommandReceivedEvent>([this](const SupDef::Events& events) {
        //     for (const auto& event : events) {
        //         const auto& typedEvent = static_cast<const SupDef::StartCommandReceivedEvent&>(*event);
        //         onStartReceivedCommand(typedEvent);
        //     }
        // });
        // globalDispatcher->subscribe<UpdateCommandReceivedEvent>([this](const SupDef::Events& events) {
        //     for (const auto& event : events) {
        //         const auto& typedEvent = static_cast<const SupDef::UpdateCommandReceivedEvent&>(*event);
        //         onUpdateReceivedCommand(typedEvent);
        //     }
        // });
        // globalDispatcher->subscribe<ConfirmCommandReceivedEvent>([this](const SupDef::Events& events) {
        //     for (const auto& event : events) {
        //         const auto& typedEvent = static_cast<const SupDef::ConfirmCommandReceivedEvent&>(*event);
        //         onConfirmReceivedCommand(typedEvent);
        //     }
        // });
        globalDispatcher->subscribe<CommandToRenderEvent>([this](const SupDef::Events& events) {
            for (const auto& event : events) {
                const auto& typedEvent = static_cast<const SupDef::CommandToRenderEvent&>(*event);
                onCommandToRender(typedEvent);
            }
        });
        
    }

    void RendererBasic::onCommandToRender(const CommandToRenderEvent& event) {
        // std::cout << "Renderer received event: CommandToRenderEvent; ";
        // std::cout << "  Command: " << event.commandID << "; data: " << event.data.dump(4) << "\n";
        if (!game) return;
        if (!game->getAssetManager()) return;
        currentCommand = event.commandID;
        auto asset = game->getAssetManager()->getAsset(event.commandID);
        processCommand(asset, event.data);
    }

    // void RendererBasic::onStartReceivedCommand(const StartCommandReceivedEvent& event) {
    //     // if (commandMode != RCommandMode::NONE) return;
    //     std::cout << "Renderer received event: StartCommandReceivedEvent; ";
    //     std::cout << "  Command: " << event.commandID << "; Success: " << event.success << "\n";
    //     if (!event.success) resetCommand();
    //     currentCommand = event.commandID;
    //     commandMode = RCommandMode::BUILD;
    // }

    // void RendererBasic::onUpdateReceivedCommand(const UpdateCommandReceivedEvent& event) {
    //     // if (commandMode == RCommandMode::NONE) return;
    //     std::cout << "Renderer received event: UpdateCommandReceivedEvent; ";
    //     std::cout << "  Data: " << event.data.dump(4) << "\n";
    //     //
    // }

    // void RendererBasic::onConfirmReceivedCommand(const ConfirmCommandReceivedEvent& event) {
    //     // if (commandMode == RCommandMode::NONE) return;
    //     std::cout << "Renderer received event: ConfirmCommandReceivedEvent; ";
    //     std::cout << "  Success: " << event.success << "; data: " << event.data.dump(4) << "\n";
    //     if (!event.success) resetCommand();
    //     resetCommand();
    // }

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
