#include <ECS/component_registry.hpp>

#pragma once


namespace SupDef {

    struct MoveCommandComponent : public Component {
        MoveCommandComponent() { addToRegistry(); }
        
        void addToRegistry() {
            ComponentRegistry::registerComponent(getTypeName(), []()
                { return std::make_unique<MoveCommandComponent>(); });
        }

        void to_json(json& j) const override {
            // j = json{{S_TO_BUILD, toBuild}};
        }
    
        void from_json(const json& j) override {
            // j.at(S_TO_BUILD).get_to(toBuild);
        }
    
        std::string getTypeName() const override {
            return SCA_MOVE_COMMAND;
        }

        bool isAsset() const override { return true; }

    };
    
}
