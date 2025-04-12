#pragma once

#include <ECS/component_registry.hpp>
#include <Misc/text.hpp>


namespace SupDef {

    DEFINE_COMPONENT_BEGIN(InitContainerComponent, SCI_INIT_CONTAINER)
        AssetIDs contained;

        InitContainerComponent(AssetID contained_) { addToRegistry(); contained.push_back(contained_); }
        InitContainerComponent(AssetIDs contained_) { addToRegistry(); contained = contained_; }

        InitContainerComponent(AssetID contained1, AssetID contained2) {
            addToRegistry();
            contained.push_back(contained1);
            contained.push_back(contained2);
        }

        InitContainerComponent(AssetID contained1, AssetID contained2, AssetID contained3) {
            addToRegistry();
            contained.push_back(contained1);
            contained.push_back(contained2);
            contained.push_back(contained3);
        }

        InitContainerComponent(AssetID contained1, AssetID contained2, AssetID contained3, AssetID contained4) {
            addToRegistry();
            contained.push_back(contained1);
            contained.push_back(contained2);
            contained.push_back(contained3);
            contained.push_back(contained4);
        }

        bool isAsset() const override { return true; }

        REFLECT_BEGIN
            REFLECT_FIELD(contained)
        REFLECT_END
    DEFINE_COMPONENT_END
}
