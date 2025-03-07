#include <ECS/component_registry.hpp>

#pragma once


namespace SupDef {

    struct ActiveTechComponent : public Component {
        CommandID commandID = NO_COMMAND;
        Cooldown current_cooldown = 0;
        bool initWithFullCooldown = false;

        ActiveTechComponent() : commandID(NO_COMMAND) { addToRegistry(); }
        ActiveTechComponent(CommandID commandID) : commandID(commandID) { addToRegistry(); }

        void addToRegistry() {
            ComponentRegistry::registerComponent(getTypeName(), []()
                { return std::make_unique<ActiveTechComponent>(); });
        }

        void to_json(json& j) const override {
            j[S_COMMAND] = commandID;
            j[S_CURRENT_COOLDOWN] = current_cooldown;
            j[S_INIT_FULL_COOLDOWN] = initWithFullCooldown;
        }

        void from_json(const json& j) override {
            j.at(S_COMMAND).get_to(commandID);
            j.at(S_CURRENT_COOLDOWN).get_to(current_cooldown);
            j.at(S_INIT_FULL_COOLDOWN).get_to(initWithFullCooldown);
        }

        std::string getTypeName() const override {
            return SCT_ACTIVE_TECH;
        }

        bool isTech() const override { return true; }

    };

}
