#pragma once

#include <ECS/collision/bounding_box.hpp>


namespace SupDef {
    
    class CollisionShape {
        public:
            float offsetX = 0.0f, offsetY = 0.0f;
            virtual ~CollisionShape() = default;
            virtual BoundingBox getBoundingBox() = 0;
    };

    using UCollisionShape = std::unique_ptr<CollisionShape>;
    using CollisionShapes = std::vector<UCollisionShape>;

}
