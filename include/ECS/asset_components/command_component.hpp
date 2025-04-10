#pragma once

#include <ECS/component_registry.hpp>


namespace SupDef {

    DEFINE_COMPONENT_BEGIN(CommandComponent, SCA_COMMAND)
        bool isUnique = false;
        Cooldown cooldown = NO_COOLDOWN;
        
        CommandComponent(bool unique) : isUnique(unique) { addToRegistry(); }
        CommandComponent(bool unique, Cooldown cooldown) : isUnique(unique), cooldown(cooldown) { addToRegistry(); }
        
        bool isAsset() const override { return true; }

        REFLECT_COMPONENT_BEGIN(ThisType)
            REFLECT_FIELD(isUnique)
            REFLECT_FIELD(cooldown)
        REFLECT_COMPONENT_END()
    DEFINE_COMPONENT_END

}
