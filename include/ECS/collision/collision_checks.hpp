#include <ECS/collision/collisions_shapes.hpp>

#pragma once


namespace SupDef::CollisionUtils {

    // Circle vs. Circle
    bool checkCollision(const CircleShape* c1, float x1, float y1, const CircleShape* c2, float x2, float y2) {
        return Math::circleOverlap(x1, y1, x2, y2, c1->radius, c2->radius);
    }

    // Rectangle vs. Rectangle
    bool checkCollision(const RectangleShape* r1, float x1, float y1, const RectangleShape* r2, float x2, float y2) {
        return !(x1 + r1->width  <= x2 || x2 + r2->width  <= x1 ||
                 y1 + r1->height <= y2 || y2 + r2->height <= y1);
    }

    // Circle vs. Rectangle (using nearest point projection)
    bool checkCollision(const CircleShape* circle, float cx, float cy, const RectangleShape* rect, float rx, float ry) {
        float closestX = std::max(rx, std::min(cx, rx + rect->width));
        float closestY = std::max(ry, std::min(cy, ry + rect->height));
        float dx = cx - closestX;
        float dy = cy - closestY;
        return (dx * dx + dy * dy) <= (circle->radius * circle->radius);
    }

    // General collision check
    bool checkCollision(const CollisionShape* s1, float x1, float y1, const CollisionShape* s2, float x2, float y2) {
        if (const auto* c1 = dynamic_cast<const CircleShape*>(s1)) {
            if (const auto* c2 = dynamic_cast<const CircleShape*>(s2)) {
                std::cout << "    Circle circle\n";
                std::cout << "      " << x1 << "; " << y1 << "; " << c1->radius << "\n";
                std::cout << "      " << x2 << "; " << y2 << "; " << c2->radius << "\n";
                return checkCollision(c1, x1, y1, c2, x2, y2);
            } else if (const auto* r2 = dynamic_cast<const RectangleShape*>(s2)) {
                return checkCollision(c1, x1, y1, r2, x2, y2);
            }
        } else if (const auto* r1 = dynamic_cast<const RectangleShape*>(s1)) {
            if (const auto* c2 = dynamic_cast<const CircleShape*>(s2)) {
                return checkCollision(c2, x2, y2, r1, x1, y1);
            } else if (const auto* r2 = dynamic_cast<const RectangleShape*>(s2)) {
                return checkCollision(r1, x1, y1, r2, x2, y2);
            }
        }
        return false;
    }

}