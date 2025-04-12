#pragma once

#include <ECS/component_registry.hpp>


namespace SupDef {

    DEFINE_COMPONENT_BEGIN(GraphicAssetComponent, SCA_GRAPHIC_ASSET)
        std::string filepath = "";

        bool isAsset() const override { return true; }

        REFLECT_BEGIN
            REFLECT_FIELD(filepath)
        REFLECT_END
    DEFINE_COMPONENT_END
    
}
