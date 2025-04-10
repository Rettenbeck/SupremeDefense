#pragma once

#include <ECS/collision/collision_shape.hpp>


namespace SupDef {
    
    class RectangleShape : public CollisionShape {
        public:
            float width, height;

            RectangleShape(float offsetX_, float offsetY_, float w, float h) : width(w), height(h) {
                offsetX = offsetX_;
                offsetY = offsetY_;
            }
        
            RectangleShape(float w, float h) : width(w), height(h) {
                offsetX = 0.0;
                offsetY = 0.0;
            }
        
            BoundingBox getBoundingBox() {
                return BoundingBox(offsetX, offsetY, width, height);
            }

    };

}
