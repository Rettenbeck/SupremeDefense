#pragma once

#include <ECS/component_registry.hpp>


namespace SupDef {

    DEFINE_COMPONENT_BEGIN(GraphicComponent, SC_GRAPHIC)
        std::string filepath = "";
        bool drawCentered = true;
        long zBuffer = 1000;

        GraphicComponent(std::string filepath) : filepath(filepath) {
            addToRegistry();
        }
        
        GraphicComponent(std::string filepath, long zBuffer, bool drawCentered = true)
        : filepath(filepath), zBuffer(zBuffer), drawCentered(drawCentered) {
            addToRegistry();
        }
        
        REFLECT_BEGIN
            REFLECT_FIELD(filepath)
            REFLECT_FIELD(drawCentered)
            REFLECT_FIELD(zBuffer)
        REFLECT_END
    DEFINE_COMPONENT_END

}
