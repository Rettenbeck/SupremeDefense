#pragma once

#include <ECS/component_registry.hpp>


namespace SupDef {

    DEFINE_COMPONENT_BEGIN(InitPositionComponent, SCI_INIT_POSITION)
        float x = 0, y = 0;
        bool relativeToMap = true;

        InitPositionComponent(float x, float y) : x(x), y(y) { addToRegistry(); }
        
        bool isAsset() const override { return true; }

        REFLECT_BEGIN
            REFLECT_FIELD(x)
            REFLECT_FIELD(y)
            REFLECT_FIELD(relativeToMap)
        REFLECT_END
    DEFINE_COMPONENT_END
}
