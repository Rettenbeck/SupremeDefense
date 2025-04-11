#pragma once

#include <ECS/field.hpp>
#include <ECS/constants.hpp>
#include <nlohmann/include.hpp>


namespace SupDef {

    DEFINE_SCLASS_BEGIN(WaveSegment)
        AssetID toSpawn = NO_ASSET;
        float xRel = 0.0, yRel = 0.0;
        long frameCount = 0;

        WaveSegment(AssetID toSpawn, long frameCount) : toSpawn(toSpawn), frameCount(frameCount) {}
        WaveSegment(AssetID toSpawn, long frameCount, float xRel, float yRel)
        : toSpawn(toSpawn), frameCount(frameCount), xRel(xRel), yRel(yRel) {}

        REFLECT_COMPONENT_BEGIN(ThisType)
            REFLECT_FIELD(toSpawn)
            REFLECT_FIELD(xRel)
            REFLECT_FIELD(yRel)
        REFLECT_COMPONENT_END()
    DEFINE_SCLASS_END

    DEFINE_UNIQUE_AND_LIST(WaveSegment, UWaveSegment, WaveSegments);

}
