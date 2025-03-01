#include <ECS/component_registry.hpp>

#pragma once


namespace SupDef {

    struct TeamComponent : public Component {
        // EntityID teamID = NO_ENTITY;
        
        TeamComponent() { addToRegistry(); }

        void addToRegistry() {
            ComponentRegistry::registerComponent(getTypeName(), []()
                { return std::make_unique<TeamComponent>(); });
        }

        void to_json(json& j) const override {
            // j = json{{S_TEAM, teamID}};
        }
    
        void from_json(const json& j) override {
            // j.at(S_TEAM).get_to(teamID);
        }
    
        std::string getTypeName() const override {
            return SC_TEAM;
        }
    };
    
}
