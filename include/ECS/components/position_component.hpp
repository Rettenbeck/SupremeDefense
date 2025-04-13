#pragma once

#include <ECS/component_registry.hpp>


namespace SupDef {

    DEFINE_COMPONENT_BEGIN(PositionComponent, SC_POSITION)
        float x = 0.0f, y = 0.0f;
        float ox = 0.0f, oy = 0.0f;
        float xRel = 0.0f, yRel = 0.0f;
        bool followParent = false;
        float lastAngle = -1.0f;

        PositionComponent(bool followParent) : followParent(followParent) { addToRegistry(); }
        PositionComponent(float x_, float y_) : x(x_), y(y_) { addToRegistry(); }

        void storeOldPosition() { ox = x; oy = y; }

        float getAngleOfVelocity() {
            auto dx = x - ox; auto dy = y - oy;
            if (dx == 0.0 && dy == 0.0) return -1.0;
            double angle = std::atan2(dy, dx);
            lastAngle = angle;
            return angle;
        }

        REFLECT_BEGIN
            REFLECT_FIELD(x)
            REFLECT_FIELD(y)
            REFLECT_FIELD(ox)
            REFLECT_FIELD(oy)
            REFLECT_FIELD(xRel)
            REFLECT_FIELD(yRel)
            REFLECT_FIELD(followParent)
        REFLECT_END
    DEFINE_COMPONENT_END

}
