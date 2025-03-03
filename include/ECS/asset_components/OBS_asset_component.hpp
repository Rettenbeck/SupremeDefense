#include <ECS/component_registry.hpp>

#pragma once


namespace SupDef {

    struct AssetComponent : public Component {
        AssetID assetID = NO_ASSET;
        std::string name = "";
        std::string desc = "";
        
        AssetComponent() { addToRegistry(); }
        AssetComponent(AssetID id, std::string name, std::string desc)
            : assetID(std::move(id)), name(std::move(name)), desc(std::move(desc)) { addToRegistry(); }

        void addToRegistry() {
            ComponentRegistry::registerComponent(getTypeName(), []()
                { return std::make_unique<AssetComponent>(); });
        }

        void to_json(json& j) const override {
            //j = json{{S_ID, parent}};
        }
    
        void from_json(const json& j) override {
            //j.at(S_PARENT).get_to(parent);
        }
    
        std::string getTypeName() const override {
            return SCA_ASSET;
        }

        bool isAssetOnly() const override { return true; }

    };
    
}
