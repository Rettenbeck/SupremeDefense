#include <ECS/component_registry.hpp>
#include <Misc/text.hpp>

#pragma once


namespace SupDef {

    struct InitContainerComponent : public Component {
        AssetIDs contained;

        InitContainerComponent() { addToRegistry(); }
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

        void addToRegistry() {
            ComponentRegistry::registerComponent(getTypeName(), []()
                { return std::make_unique<InitContainerComponent>(); });
        }

        void to_json(json& j) const override {
            j = json{{S_CONTAINED, contained}};
        }
    
        void from_json(const json& j) override {
            contained.clear();
            j.at(S_CONTAINED).get_to(contained);
        }
    
        std::string getTypeName() const override {
            return SCA_INIT_CONTAINER;
        }

        bool isAsset() const override { return true; }

    };
    
}
