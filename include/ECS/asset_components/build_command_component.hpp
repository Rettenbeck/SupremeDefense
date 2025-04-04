#include <ECS/component_registry.hpp>

#pragma once


namespace SupDef {

    DEFINE_COMPONENT_BEGIN(BuildCommandComponent, SCA_BUILD_COMMAND)
        AssetID toBuild;
        
        BuildCommandComponent(AssetID toBuild) : toBuild(toBuild) { addToRegistry(); }
        
        bool isAsset() const override { return true; }

        REFLECT_COMPONENT_BEGIN(ThisType)
            REFLECT_FIELD(toBuild)
        REFLECT_COMPONENT_END()
    DEFINE_COMPONENT_END
}
