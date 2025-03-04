#include <ECS/include.hpp>

#pragma once


namespace SupDef {

    enum class CommandStatus {
        NONE,
        RECEIVED,
        ONGOING,
        CONFIRMED,
        CANCELLED
    };

    class CommandProcessor {
        public:
            CommandProcessor(EventDispatcher* dispatcher) : globalDispatcher(dispatcher) {
                subscribeToEvents();
            }
        
            void subscribeToEvents() {
                globalDispatcher->subscribe<StartCommandEvent>([this](const SupDef::Events& events) {
                    for (const auto& event : events) {
                        const auto& typedEvent = static_cast<const SupDef::StartCommandEvent&>(*event);
                        onStartCommand(typedEvent);
                    }
                });
                globalDispatcher->subscribe<UpdateCommandEvent>([this](const SupDef::Events& events) {
                    for (const auto& event : events) {
                        const auto& typedEvent = static_cast<const SupDef::UpdateCommandEvent&>(*event);
                        onUpdateCommand(typedEvent);
                    }
                });
                globalDispatcher->subscribe<ConfirmCommandEvent>([this](const SupDef::Events& events) {
                    for (const auto& event : events) {
                        const auto& typedEvent = static_cast<const SupDef::ConfirmCommandEvent&>(*event);
                        onConfirmCommand(typedEvent);
                    }
                });
            }

            void onStartCommand(const StartCommandEvent& event) {
                // if (commandStatus != CommandStatus::NONE) {
                //     Logger::getInstance().addMessage(MessageType::Error, "A command is already active!");
                //     return;
                // }
                reset();
                std::cout << "ComProcessor received event: StartCommandEvent; ";
                std::cout << "  Command: " << event.commandID << "\n";
                currentCommand = event.commandID;
                commandStatus = CommandStatus::RECEIVED;
            }

            void onUpdateCommand(const UpdateCommandEvent& event) {
                if (commandStatus != CommandStatus::RECEIVED && commandStatus != CommandStatus::ONGOING) return;
                commandStatus = CommandStatus::ONGOING;
                data = event.data;
            }

            void onConfirmCommand(const ConfirmCommandEvent& event) {
                if (commandStatus != CommandStatus::RECEIVED && commandStatus != CommandStatus::ONGOING) return;
                std::cout << "ComProcessor received event: ConfirmCommandEvent; ";
                std::cout << "  Confirmed: " << event.isConfirmed << "; data: " << event.data.dump(4) << "\n";
            
                if (event.isConfirmed) {
                    commandStatus = CommandStatus::CONFIRMED;
                } else {
                    commandStatus = CommandStatus::CANCELLED;
                }
            }
            
            CommandID getCurrentCommand() const { return currentCommand; }
            CommandStatus getCommandStatus() const { return commandStatus; }
            void setCommandStatus(CommandStatus status) { commandStatus = status; }
            json& getData() { return data; }
        
            void reset() {
                currentCommand = NO_COMMAND;
                commandStatus = CommandStatus::NONE;
                data.clear();
            }
        
        private:
            EventDispatcher* globalDispatcher;
            CommandID currentCommand = NO_COMMAND;
            CommandStatus commandStatus = CommandStatus::NONE;
            json data;

    };
    
    using UComProcessor = std::unique_ptr<CommandProcessor>;

}
