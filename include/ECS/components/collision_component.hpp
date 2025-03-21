#include <ECS/component_registry.hpp>
#include <ECS/collision/collision_checks.hpp>

#pragma once


namespace SupDef {

    struct CollisionComponent : public Component {
        CollisionShapes shapes;
        BoundingBox boundingBox;
        float dummyRadius = 0.0f;
    
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
        
        bool collidesWithSimple(const CollisionComponent* other, float myX, float myY, float otherX, float otherY) const {
            if(dummyRadius == 0.0 || other->dummyRadius == 0.0) return false;
            return Math::circleOverlap(myX, myY, otherX, otherY, dummyRadius, other->dummyRadius);
        }
        
        bool collidesWith(const CollisionComponent* other, float myX, float myY, float otherX, float otherY) const {
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

        void to_json(json& j) const override {
            j[S_DUMMY_RADIUS] = dummyRadius;
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
