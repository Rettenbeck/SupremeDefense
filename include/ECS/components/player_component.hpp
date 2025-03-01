#include <ECS/component_registry.hpp>

#pragma once


namespace SupDef {

    struct PlayerComponent : public Component {
        EntityID teamID = NO_ENTITY;
        
        PlayerComponent() { addToRegistry(); }

        void addToRegistry() {
            ComponentRegistry::registerComponent(getTypeName(), []()
                { return std::make_unique<PlayerComponent>(); });
        }

        void to_json(json& j) const override {
            j = json{{S_TEAM, teamID}};
        }
    
        void from_json(const json& j) override {
            j.at(S_TEAM).get_to(teamID);
        }
    
        std::string getTypeName() const override {
            return SC_PLAYER;
        }
    };
    
}
