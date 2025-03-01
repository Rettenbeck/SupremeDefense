#include <ECS/collision/bounding_box.hpp>

#pragma once


namespace SupDef {
    
    class CollisionShape {
        public:
            float offsetX = 0.0f, offsetY = 0.0f;
            virtual ~CollisionShape() = default;
            virtual BoundingBox getBoundingBox();
    };

    using UCollisionShape = std::unique_ptr<CollisionShape>;
    using CollisionShapes = std::vector<UCollisionShape>;

}
