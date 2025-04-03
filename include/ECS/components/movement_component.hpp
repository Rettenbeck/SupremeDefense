#include <ECS/component_registry.hpp>

#pragma once


namespace SupDef {

    enum class MovementMode {
        PursueTarget, DirectedMotion
    };

    struct MovementComponent : public Component {
        float vx = 0.0f, vy = 0.0f;                 // Current x and y speed; calculated by direction and speed
        float original_speed = 0.0f;                // Speed without any modifiers
        float speed = 0.0f;                         // Current speed with modifiers applied
        bool isGroundBased = true;                  // Ground-based = bound to tiles (has to avoid walls and buildings)
        bool hasGoal = false;                       // Has currently a goal to follow
        float goalX = 0.0f, goalY = 0.0f;           // X and Y of goal
        float tempGoalX = 0.0f, tempGoalY = 0.0f;   // X and Y of temporary goal (relevant only for ground-based movers as flyers can move straigth to target)
        MovementMode movementMode = MovementMode::PursueTarget;

        MovementComponent(float speed_, bool isGroundBased_) : original_speed(speed_), isGroundBased(isGroundBased_) { addToRegistry(); }
        MovementComponent() { addToRegistry(); }

        void addToRegistry() {
            ComponentRegistry::registerComponent(getTypeName(), []()
                { return std::make_unique<MovementComponent>(); });
        }

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
        
        void to_json(json& j) const override {
            generic_to_json(j, this);
        }

        void from_json(const json& j) override {
            generic_from_json(j, this);
        }

        REFLECT_COMPONENT_BEGIN(MovementComponent)
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

        // void to_json(json& j) const override {
        //     j = json{
        //         {S_VX, vx},
        //         {S_VY, vy},
        //         {S_ORIGINAL_SPEED, original_speed},
        //         {S_SPEED, speed},
        //         {S_IS_GROUND_BASED, isGroundBased},
        //         {S_HAS_GOAL, hasGoal},
        //         {S_GOAL_X, goalX},
        //         {S_GOAL_Y, goalY},
        //         {S_TEMP_GOAL_X, tempGoalX},
        //         {S_TEMP_GOAL_Y, tempGoalY},
        //         {S_MOVEMENT_MODE, movementMode}
        //     };
        // }

        // void from_json(const json& j) override {
        //     j.at(S_VX).get_to(vx);
        //     j.at(S_VY).get_to(vy);
        //     j.at(S_ORIGINAL_SPEED).get_to(original_speed);
        //     j.at(S_SPEED).get_to(speed);
        //     j.at(S_IS_GROUND_BASED).get_to(isGroundBased);
        //     j.at(S_HAS_GOAL).get_to(hasGoal);
        //     j.at(S_GOAL_X).get_to(goalX);
        //     j.at(S_GOAL_Y).get_to(goalY);
        //     j.at(S_TEMP_GOAL_X).get_to(tempGoalX);
        //     j.at(S_TEMP_GOAL_Y).get_to(tempGoalY);
        //     j.at(S_MOVEMENT_MODE).get_to(movementMode);
        // }

        std::string getTypeName() const override {
            return SC_MOVEMENT;
        }

    };

}
