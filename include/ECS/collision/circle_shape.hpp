#pragma once

#include <ECS/collision/collision_shape.hpp>


namespace SupDef {
    
    class CircleShape : public CollisionShape {
        public:
            float radius;

            CircleShape(float offsetX_, float offsetY_, float r) : radius(r) {
                offsetX = offsetX_;
                offsetY = offsetY_;
            }

            CircleShape(float r) : radius(r) {
                offsetX = 0.0;
                offsetY = 0.0;
            }

            BoundingBox getBoundingBox() {
                auto [x, y, w, h] = Math::getBoundingBoxFromCircle(offsetX, offsetY, radius);
                return BoundingBox(x, y, w, h);
            }

    };

}
