#include <ECS/component_registry.hpp>

#pragma once


namespace SupDef {

    struct BuildCommandComponent : public Component {
        AssetID toBuild;
        
        BuildCommandComponent() { addToRegistry(); }
        BuildCommandComponent(AssetID toBuild) : toBuild(toBuild) { addToRegistry(); }
        
        void addToRegistry() {
            ComponentRegistry::registerComponent(getTypeName(), []()
                { return std::make_unique<BuildCommandComponent>(); });
        }

        void to_json(json& j) const override {
            j = json{{S_TO_BUILD, toBuild}};
        }
    
        void from_json(const json& j) override {
            j.at(S_TO_BUILD).get_to(toBuild);
        }
    
        std::string getTypeName() const override {
            return SCA_BUILD_COMMAND;
        }

        bool isAssetOnly() const override { return true; }

    };
    
}
