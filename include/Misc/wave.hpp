#pragma once

#include <Misc/wave_segment.hpp>


namespace SupDef {

    DEFINE_SCLASS_BEGIN(Wave)
        WaveSegments waveSegments;
        long lastIndex = -1;

        WaveSegment* getWaveSegment(long frameCount) {
            if (lastIndex == -1) {
                for (auto& waveSegment : waveSegments) {
                    lastIndex++;
                    if (waveSegment->frameCount == frameCount) {
                        return waveSegment.get();
                    }
                    if (waveSegment->frameCount > frameCount) {
                        lastIndex--;
                        return nullptr;
                    }
                }
            } else {
                while (true) {
                    if (lastIndex + 1 >= waveSegments.size()) return nullptr;
                    lastIndex++;
                    if (lastIndex >= waveSegments.size()) {
                        lastIndex--;
                        return nullptr;
                    }
                    auto& waveSegment = waveSegments[lastIndex];
                    if (waveSegment->frameCount == frameCount) return waveSegment.get();
                    if (waveSegment->frameCount > frameCount) {
                        lastIndex--;
                        return nullptr;
                    }
                }
            }
            return nullptr;
        }

        REFLECT_BEGIN
            REFLECT_LIST_UNIQUE(waveSegments, WaveSegment)
        REFLECT_END
    DEFINE_SCLASS_END

    DEFINE_UNIQUE_AND_LIST(Wave, UWave, Waves);

}
