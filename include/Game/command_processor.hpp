#include <ECS/include.hpp>

#pragma once


namespace SupDef {

    enum class CommandStatus {
        NONE,
        RECEIVED,
        ONGOING,
        CONFIRMED
    };

    class CommandProcessor {
        public:
            CommandProcessor(EventDispatcher* dispatcher) : eventDispatcher(dispatcher) {
                subscribeToEvents();
            }
        
            void subscribeToEvents() {
                eventDispatcher->subscribe<StartCommandEvent>([this](const SupDef::Events& events) {
                    for (const auto& event : events) {
                        const auto& typedEvent = static_cast<const SupDef::StartCommandEvent&>(*event);
                        onStartCommand(typedEvent);
                    }
                });
                eventDispatcher->subscribe<UpdateCommandEvent>([this](const SupDef::Events& events) {
                    for (const auto& event : events) {
                        const auto& typedEvent = static_cast<const SupDef::UpdateCommandEvent&>(*event);
                        onUpdateCommand(typedEvent);
                    }
                });
                eventDispatcher->subscribe<ConfirmCommandEvent>([this](const SupDef::Events& events) {
                    for (const auto& event : events) {
                        const auto& typedEvent = static_cast<const SupDef::ConfirmCommandEvent&>(*event);
                        onConfirmCommand(typedEvent);
                    }
                });
            }
            
            void onStartCommand(const StartCommandEvent& event) {
                if (commandStatus != CommandStatus::NONE) {
                    Logger::getInstance().addMessage(MessageType::Error, "A command is already active!");
                    return;
                }
                currentCommand = event.commandID;
                commandStatus = CommandStatus::RECEIVED;
            }

            void onUpdateCommand(const UpdateCommandEvent& event) {
                if (commandStatus != CommandStatus::RECEIVED && commandStatus != CommandStatus::ONGOING) return;
                commandStatus = CommandStatus::ONGOING;
                data = event.data;
            }

            void onConfirmCommand(const ConfirmCommandEvent& event) {
                if (commandStatus != CommandStatus::ONGOING) return;
            
                if (event.isConfirmed) {
                    commandStatus = CommandStatus::CONFIRMED;
                } else {
                    reset();
                    eventDispatcher->dispatch<ConfirmCommandReceivedEvent>(false, json());
                }
            }
            
            CommandID getCurrentCommand() const { return currentCommand; }
            CommandStatus getCommandStatus() const { return commandStatus; }
            json& getData() { return data; }
        
            void reset() {
                currentCommand = NO_COMMAND;
                commandStatus = CommandStatus::NONE;
            }
        
        private:
            EventDispatcher* eventDispatcher;
            CommandID currentCommand = NO_COMMAND;
            CommandStatus commandStatus = CommandStatus::NONE;
            json data;

    };
    
    using UComProcessor = std::unique_ptr<CommandProcessor>;

}
