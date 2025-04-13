#pragma once

#include <ECS/component_registry.hpp>


namespace SupDef {

    DEFINE_COMPONENT_BEGIN(GraphicComponent, SCF_GRAPHIC)
        std::string filepath = "";
        bool drawCentered = true;
        long zBuffer = 1000;
        bool rotate = false;

        GraphicComponent(std::string filepath) : filepath(filepath) {
            addToRegistry();
        }
        
        GraphicComponent(std::string filepath, long zBuffer, bool drawCentered = true, bool rotate = false)
        : filepath(filepath), zBuffer(zBuffer), drawCentered(drawCentered), rotate(rotate) {
            addToRegistry();
        }
        
        REFLECT_BEGIN
            REFLECT_FIELD(filepath)
            REFLECT_FIELD(drawCentered)
            REFLECT_FIELD(zBuffer)
            REFLECT_FIELD(rotate)
        REFLECT_END
    DEFINE_COMPONENT_END

}
