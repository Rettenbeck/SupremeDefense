#pragma once

#include <ECS/component_registry.hpp>


namespace SupDef {

    DEFINE_COMPONENT_BEGIN(BuildCommandComponent, SCA_BUILD_COMMAND)
        AssetID toBuild;
        
        BuildCommandComponent(AssetID toBuild) : toBuild(toBuild) { addToRegistry(); }
        
        bool isAsset() const override { return true; }

        REFLECT_BEGIN
            REFLECT_FIELD(toBuild)
        REFLECT_END
    DEFINE_COMPONENT_END
}
