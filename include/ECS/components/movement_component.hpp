#include <ECS/component_registry.hpp>

#pragma once


namespace SupDef {

    struct MovementComponent : public Component {
        float vx = 0.0f, vy = 0.0f;
        float original_speed = 0.0f;
        float speed = 0.0f;
        bool isGroundBased = true;
        bool hasGoal = false;
        float goalX = 0.0f, goalY = 0.0f;
        float tempGoalX = 0.0f, tempGoalY = 0.0f;

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
            //setVelocityTowardsGoal(myX, myY);
        }

        bool isCloseToGoal(float myX, float myY, float toleranceSquared = 1.0f) {
            float targetX = isGroundBased ? tempGoalX : goalX;
            float targetY = isGroundBased ? tempGoalY : goalY;
            return Math::getDistanceSquared(myX, myY, targetX, targetY) <= toleranceSquared;
        }
        
        void to_json(json& j) const override {
            j = json{
                {S_VX, vx},
                {S_VY, vy},
                {S_ORIGINAL_SPEED, original_speed},
                {S_SPEED, speed},
                {S_IS_GROUND_BASED, isGroundBased},
                {S_HAS_GOAL, hasGoal},
                {S_GOAL_X, goalX},
                {S_GOAL_Y, goalY},
                {S_TEMP_GOAL_X, tempGoalX},
                {S_TEMP_GOAL_Y, tempGoalY}
            };
        }

        void from_json(const json& j) override {
            j.at(S_VX).get_to(vx);
            j.at(S_VY).get_to(vy);
            j.at(S_ORIGINAL_SPEED).get_to(original_speed);
            j.at(S_SPEED).get_to(speed);
            j.at(S_IS_GROUND_BASED).get_to(isGroundBased);
            j.at(S_HAS_GOAL).get_to(hasGoal);
            j.at(S_GOAL_X).get_to(goalX);
            j.at(S_GOAL_Y).get_to(goalY);
            j.at(S_TEMP_GOAL_X).get_to(tempGoalX);
            j.at(S_TEMP_GOAL_Y).get_to(tempGoalY);
        }

        std::string getTypeName() const override {
            return SC_MOVEMENT;
        }

    };

}
