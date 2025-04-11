#pragma once

#include <ECS/component_registry.hpp>


namespace SupDef {

    DEFINE_COMPONENT_BEGIN(MapComponent, SC_MAP)
        int width = 0, height = 0;

        MapComponent(int width_, int height_) : width(width_), height(height_) { addToRegistry(); }

        REFLECT_COMPONENT_BEGINX
            REFLECT_FIELD(width)
            REFLECT_FIELD(height)
        REFLECT_COMPONENT_END()
    DEFINE_COMPONENT_END
    
}
