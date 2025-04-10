#pragma once

#include <ECS/component_registry.hpp>


namespace SupDef {

    enum class MovementMode {
        PursueTarget, DirectedMotion
    };

    DEFINE_COMPONENT_BEGIN(MovementComponent, SC_MOVEMENT)
        float vx = 0.0f, vy = 0.0f;                 // Current x and y speed; calculated by direction and speed
        float original_speed = 0.0f;                // Speed without any modifiers
        float speed = 0.0f;                         // Current speed with modifiers applied
        bool isGroundBased = true;                  // Ground-based = bound to tiles (has to avoid walls and buildings)
        bool hasGoal = false;                       // Has currently a goal to follow
        float goalX = 0.0f, goalY = 0.0f;           // X and Y of goal
        float tempGoalX = 0.0f, tempGoalY = 0.0f;   // X and Y of temporary goal (relevant only for ground-based movers as flyers can move straigth to target)
        MovementMode movementMode = MovementMode::PursueTarget;

        MovementComponent(float speed_, bool isGroundBased_) : original_speed(speed_), isGroundBased(isGroundBased_) { addToRegistry(); }

        void setCurrentSpeed() {
            speed = original_speed;
        }

        void clearGoal() { hasGoal = false; }

        void setVelocityToZero() { vx = 0.0f; vy = 0.0f; }

        void setVelocityTowardsTarget(float myX, float myY, float targetX, float targetY) {
            setVelocityToZero();
            setCurrentSpeed();
            float dx = targetX - myX;
            float dy = targetY - myY;
            float length = std::sqrt(dx * dx + dy * dy);
            if (length > 0.0f) {
                vx = (dx / length) * speed;
                vy = (dy / length) * speed;
            }
        }
        
        void setVelocityTowardsGoal(float myX, float myY) {
            if (!hasGoal) {
                setVelocityToZero();
                return;
            }
            setCurrentSpeed();
            
            float targetX = isGroundBased ? tempGoalX : goalX;
            float targetY = isGroundBased ? tempGoalY : goalY;
            
            float dx = targetX - myX;
            float dy = targetY - myY;
            float length = std::sqrt(dx * dx + dy * dy);
            
            if (length > 0.0f) {
                vx = (dx / length) * speed;
                vy = (dy / length) * speed;
            }
        }
        
        void setGoal(float goalX_, float goalY_, float myX, float myY) {
            goalX = goalX_; goalY = goalY_;
            hasGoal = true;
        }

        bool isCloseToGoal(float myX, float myY, float toleranceSquared = 1.0f) {
            float targetX = isGroundBased ? tempGoalX : goalX;
            float targetY = isGroundBased ? tempGoalY : goalY;
            return Math::getDistanceSquared(myX, myY, targetX, targetY) <= toleranceSquared;
        }
        
        REFLECT_COMPONENT_BEGIN(ThisType)
            REFLECT_FIELD(vx)
            REFLECT_FIELD(vy)
            REFLECT_FIELD(original_speed)
            REFLECT_FIELD(speed)
            REFLECT_FIELD(isGroundBased)
            REFLECT_FIELD(hasGoal)
            REFLECT_FIELD(goalX)
            REFLECT_FIELD(goalY)
            REFLECT_FIELD(tempGoalX)
            REFLECT_FIELD(tempGoalY)
            REFLECT_FIELD(movementMode)
        REFLECT_COMPONENT_END()
    DEFINE_COMPONENT_END
}
