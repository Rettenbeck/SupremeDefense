#pragma once

#include <ECS/component_registry.hpp>


namespace SupDef {

    DEFINE_COMPONENT_BEGIN(PositionComponent, SC_POSITION)
        float x = 0.0f, y = 0.0f;
        float xRel = 0.0f, yRel = 0.0f;
        bool followParent = false;

        PositionComponent(bool followParent) : followParent(followParent) { addToRegistry(); }
        PositionComponent(float x_, float y_) : x(x_), y(y_) { addToRegistry(); }

        REFLECT_COMPONENT_BEGINX
            REFLECT_FIELD(x)
            REFLECT_FIELD(y)
            REFLECT_FIELD(xRel)
            REFLECT_FIELD(yRel)
            REFLECT_FIELD(followParent)
        REFLECT_COMPONENT_END()
    DEFINE_COMPONENT_END

}
