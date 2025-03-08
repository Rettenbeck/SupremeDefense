#include <ECS/include.hpp>

#pragma once


namespace SupDef {

    class CommandProcessor {
        public:
            CommandProcessor(EventDispatcher* dispatcher) : globalDispatcher(dispatcher) {
                subscribeToEvents();
            }
        
            void subscribeToEvents() {
                // globalDispatcher->subscribe<TriggerCommandEvent>([this](const SupDef::Events& events) {
                //     for (const auto& event : events) {
                //         const auto& typedEvent = static_cast<const SupDef::TriggerCommandEvent&>(*event);
                //         onStartCommand(typedEvent);
                //     }
                // });
                // globalDispatcher->subscribe<UpdateCommandEvent>([this](const SupDef::Events& events) {
                //     for (const auto& event : events) {
                //         const auto& typedEvent = static_cast<const SupDef::UpdateCommandEvent&>(*event);
                //         onUpdateCommand(typedEvent);
                //     }
                // });
                // globalDispatcher->subscribe<ConfirmCommandEvent>([this](const SupDef::Events& events) {
                //     for (const auto& event : events) {
                //         const auto& typedEvent = static_cast<const SupDef::ConfirmCommandEvent&>(*event);
                //         onConfirmCommand(typedEvent);
                //     }
                // });
                // globalDispatcher->subscribe<DirectCommandEvent>([this](const SupDef::Events& events) {
                //     for (const auto& event : events) {
                //         const auto& typedEvent = static_cast<const SupDef::DirectCommandEvent&>(*event);
                //         directs.push_back(typedEvent);
                //     }
                // });
            }

            void onStartCommand(const StartCommandEvent& event) {
                // reset();
                // std::cout << "ComProcessor received event: StartCommandEvent; ";
                // std::cout << "  Command: " << event.commandID << "\n";
                // currentCommand = event.commandID;
                // commandStatus = CommandStatus::RECEIVED;
                // data = event.data;
            }

            void onUpdateCommand(const UpdateCommandEvent& event) {
                // if (commandStatus != CommandStatus::RECEIVED && commandStatus != CommandStatus::ONGOING) return;
                // commandStatus = CommandStatus::ONGOING;
                // data = event.data;
            }

            void onConfirmCommand(const ConfirmCommandEvent& event) {
                // if (commandStatus != CommandStatus::RECEIVED && commandStatus != CommandStatus::ONGOING) return;
                // std::cout << "ComProcessor received event: ConfirmCommandEvent; ";
                // std::cout << "  Confirmed: " << event.isConfirmed << "; data: " << event.data.dump(4) << "\n";
            
                // if (event.isConfirmed) {
                //     commandStatus = CommandStatus::CONFIRMED;
                // } else {
                //     commandStatus = CommandStatus::CANCELLED;
                // }
                // data = event.data;
            }
            
            CommandID getCurrentCommand() const { return currentCommand; }
            CommandStatus getCommandStatus() const { return commandStatus; }
            void setCommandStatus(CommandStatus status) { commandStatus = status; }
            std::vector<DirectCommandEvent>& getDirects() { return directs; }
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
            // std::vector<DirectCommandEvent> directs;
            json data;

    };
    
    using UComProcessor = std::unique_ptr<CommandProcessor>;

}
