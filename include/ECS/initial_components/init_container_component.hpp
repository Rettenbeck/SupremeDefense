#pragma once

#include <ECS/component_registry.hpp>
#include <Misc/text.hpp>


namespace SupDef {

    DEFINE_COMPONENT_BEGIN(InitContainerComponent, SCI_INIT_CONTAINER)
        InitContainerList contained;

        InitContainerComponent(AssetID contained_) {
            addToRegistry();
            add(contained_);
        }

        InitContainerComponent(AssetIDs contained_) {
            addToRegistry();
            add(contained_);
        }

        void add(AssetID contained_, float x, float y) {
            contained.emplace_back(contained_, true, x, y);
        }

        void add(AssetID contained_) {
            contained.emplace_back(contained_, false, 0.0, 0.0);
        }

        void add(AssetIDs contained_) {
            for (auto& item : contained_) {
                add(item);
            }
        }

        bool isAsset() const override { return true; }

        REFLECT_BEGIN
            REFLECT_FIELD(contained)
        REFLECT_END
    DEFINE_COMPONENT_END
}
