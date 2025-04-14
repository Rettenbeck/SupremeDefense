#pragma once

#include <ECS/component_registry.hpp>
#include <Misc/wave.hpp>


namespace SupDef {

    DEFINE_COMPONENT_BEGIN(EnemyWavesComponent, SCI_ENEMY_WAVES)
        Waves waves;

        EnemyWavesComponent(Waves waves_) {
            waves = std::move(waves_);
            addToRegistry();
        }

        void addSegment(long waveNumber, long frameCount, float x, float y, AssetID enemyID) {
            while (waves.size() < waveNumber) {
                waves.push_back(std::make_unique<Wave>());
            }
            assert(waves.size() >= waveNumber);
            auto& wave = waves[waveNumber - 1];
            wave->waveSegments.push_back(std::make_unique<WaveSegment>(enemyID, frameCount, x, y));
        }

        bool isAsset() const override { return true; }

        REFLECT_BEGIN
            REFLECT_LIST_UNIQUE(waves, Wave)
        REFLECT_END
    DEFINE_COMPONENT_END
}
