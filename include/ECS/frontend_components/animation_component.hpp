#pragma once

#include <ECS/component_registry.hpp>


namespace SupDef {

    enum class MovementAnimation {
        AlwaysAnimate, AnimateMovement, ResetAnimationOnStop
    };

    DEFINE_COMPONENT_BEGIN(AnimationComponent, SCF_ANIMATION)
        long currentFrame = 0;
        long totalFrames = 0;
        int width = 0, height = 0;
        int columnCount = 0;
        bool dieAfterAnimation = false;
        int animationSpeed = 2;
        MovementAnimation movementAnimation = MovementAnimation::AlwaysAnimate;

        REFLECT_BEGIN
            REFLECT_FIELD(currentFrame)
            REFLECT_FIELD(totalFrames)
            REFLECT_FIELD(width)
            REFLECT_FIELD(height)
            REFLECT_FIELD(columnCount)
            REFLECT_FIELD(dieAfterAnimation)
            REFLECT_FIELD(animationSpeed)
            REFLECT_FIELD(movementAnimation)
        REFLECT_END
    DEFINE_COMPONENT_END

}
