#pragma once

#include <ECS/component_registry.hpp>


namespace SupDef {

    DEFINE_COMPONENT_BEGIN(ActiveTechComponent, SCT_ACTIVE_TECH)
        CommandID commandID = NO_COMMAND;
        Cooldown current_cooldown = 0, cooldown = 0, original_cooldown = 0;
        bool initWithFullCooldown = false;

        ActiveTechComponent(CommandID commandID) : commandID(commandID) { addToRegistry(); }

        bool isTech() const override { return true; }

        REFLECT_COMPONENT_BEGINX
            REFLECT_FIELD(commandID)
            REFLECT_FIELD(cooldown)
            REFLECT_FIELD(current_cooldown)
            REFLECT_FIELD(original_cooldown)
            REFLECT_FIELD(initWithFullCooldown)
        REFLECT_COMPONENT_END()
    DEFINE_COMPONENT_END

}
