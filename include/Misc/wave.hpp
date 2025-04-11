#pragma once

#include <Misc/wave_segment.hpp>


namespace SupDef {

    DEFINE_SCLASS_BEGIN(Wave)
        WaveSegments waveSegments;

        REFLECT_COMPONENT_BEGINX
            REFLECT_LIST_UNIQUE(waveSegments, WaveSegment)
        REFLECT_COMPONENT_END()
    DEFINE_SCLASS_END

    DEFINE_UNIQUE_AND_LIST(Wave, UWave, Waves);

}
