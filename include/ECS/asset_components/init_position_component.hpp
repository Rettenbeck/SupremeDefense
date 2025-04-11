#pragma once

#include <ECS/component_registry.hpp>


namespace SupDef {

    DEFINE_COMPONENT_BEGIN(InitPositionComponent, SCA_INIT_POSITION)
        float x = 0, y = 0;
        bool relativeToMap = true;

        InitPositionComponent(float x, float y) : x(x), y(y) { addToRegistry(); }
        
        bool isAsset() const override { return true; }

        REFLECT_COMPONENT_BEGINX
            REFLECT_FIELD(x)
            REFLECT_FIELD(y)
            REFLECT_FIELD(relativeToMap)
        REFLECT_COMPONENT_END()
    DEFINE_COMPONENT_END
}
