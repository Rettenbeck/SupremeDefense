#pragma once

#include <ECS/component_registry.hpp>
#include <ECS/collision/collision_checks.hpp>


namespace SupDef {

    struct CollisionComponent : public Component {
        CollisionShapes shapes;
        BoundingBox boundingBox;
        float dummyRadius = 0.0f;
        bool isInfluence = false;
    
        CollisionComponent(float dummyRadius_) : dummyRadius(dummyRadius_) { onInit(); }
        CollisionComponent() { onInit(); }

        void onInit() {
            addToRegistry();
            updateBoundingBox();
        }

        void addToRegistry() {
            ComponentRegistry::registerComponent(getTypeName(), []()
                { return std::make_unique<CollisionComponent>(); });
        }

        void updateBoundingBox() {
            boundingBox.isDefined = false;
            if(!shapes.empty()) {
                for(auto& shape : shapes) {
                    boundingBox.add(shape->getBoundingBox());
                }
            } else if (dummyRadius > 0.0) {
                auto [x, y, w, h] = Math::getBoundingBoxFromCircle(0, 0, dummyRadius);
                boundingBox.isDefined = true;
                boundingBox.x = x; boundingBox.y = y; boundingBox.w = w; boundingBox.h = h;
            }
        }

        void addShape(UCollisionShape shape) {
            shapes.push_back(std::move(shape));
            updateBoundingBox();
        }

        void changeRadius(float radius) {
            dummyRadius = radius;
            updateBoundingBox();
        }
        
        VF2 getCenter() const {
            if (boundingBox.isDefined) {
                return VF2(boundingBox.x + boundingBox.w / 2.0, boundingBox.y + boundingBox.h / 2.0);
            } else {
                return VF2(0.0, 0.0);
            }
        }

        VF2 getCenter(float x, float y) const {
            auto center = getCenter();
            return VF2(x + center.x, y + center.y);
        }

        bool collidesWithSimple(const CollisionComponent* other, float myX, float myY, float otherX, float otherY) const {
            assert(other);
            if(dummyRadius == 0.0 || other->dummyRadius == 0.0) return false;
            return Math::circleOverlap(myX, myY, otherX, otherY, dummyRadius, other->dummyRadius);
        }
        
        bool collidesWith(const CollisionComponent* other, float myX, float myY, float otherX, float otherY) const {
            assert(other);
            if(shapes.empty() || other->shapes.empty()) {
                return collidesWithSimple(other, myX, myY, otherX, otherY);
            }

            for (const auto& shape : shapes) {
                for (const auto& otherShape : other->shapes) {
                    if (CollisionUtils::checkCollision(shape.get(), myX + shape->offsetX, myY + shape->offsetY,
                                                       otherShape.get(), otherX + otherShape->offsetX, otherY + otherShape->offsetY)) {
                        return true;
                    }
                }
            }
            return false;
        }

        bool withinInfluence(const CollisionComponent* other, float myX, float myY, float otherX, float otherY) const {
            assert(other);
            auto myCenter = getCenter(myX, myY);
            auto otherCenter = other->getCenter(otherX, otherY);
            float radius;
            if (isInfluence) {
                radius = dummyRadius;
            } else {
                radius = other->dummyRadius;
            }
            return Math::pointWithinCircle(myCenter.x, myCenter.y, otherCenter.x, otherCenter.y, radius);
        }

        void to_json(json& j) const override {
            j[S_DUMMY_RADIUS] = dummyRadius;
            j[S_IS_INFLUENCE] = isInfluence;
            j[S_SHAPES] = json::array();
            for (const auto& shape : shapes) {
                json shapeJson;
                if (auto* circle = dynamic_cast<CircleShape*>(shape.get())) {
                    shapeJson[S_SHAPE_TYPE] = S_SHAPE_TYPE_CIRCLE;
                    shapeJson[S_RADIUS  ] = circle->radius ;
                    shapeJson[S_OFFSET_X] = circle->offsetX;
                    shapeJson[S_OFFSET_Y] = circle->offsetY;
                } else if (auto* rect = dynamic_cast<RectangleShape*>(shape.get())) {
                    shapeJson[S_SHAPE_TYPE] = S_SHAPE_TYPE_RECT;
                    shapeJson[S_WIDTH   ] = rect->width  ;
                    shapeJson[S_HEIGHT  ] = rect->height ;
                    shapeJson[S_OFFSET_X] = rect->offsetX;
                    shapeJson[S_OFFSET_Y] = rect->offsetY;
                }
                j[S_SHAPES].push_back(shapeJson);
            }
        }
    
        void from_json(const json& j) override {
            dummyRadius = j.value(S_DUMMY_RADIUS, 0.0f);
            isInfluence = j.value(S_IS_INFLUENCE, false);
            shapes.clear();
            
            for (const auto& shapeJson : j.at(S_SHAPES)) {
                std::string type = shapeJson.at(S_SHAPE_TYPE).get<std::string>();
        
                if (type == S_SHAPE_TYPE_CIRCLE) {
                    auto shape = std::make_unique<CircleShape>(
                        shapeJson.at(S_OFFSET_X).get<float>(),
                        shapeJson.at(S_OFFSET_Y).get<float>(),
                        shapeJson.at(S_RADIUS  ).get<float>()
                    );
                    addShape(std::move(shape));
                } 
                else if (type == S_SHAPE_TYPE_RECT) {
                    auto shape = std::make_unique<RectangleShape>(
                        shapeJson.at(S_OFFSET_X).get<float>(),
                        shapeJson.at(S_OFFSET_Y).get<float>(),
                        shapeJson.at(S_WIDTH   ).get<float>(),
                        shapeJson.at(S_HEIGHT  ).get<float>()
                    );
                    addShape(std::move(shape));
                }
            }
            updateBoundingBox();
        }
    
        std::string getTypeName() const override {
            return SC_COLLISION;
        }
        
    };

    using PCollisionComponents = std::vector<CollisionComponent>;

}
