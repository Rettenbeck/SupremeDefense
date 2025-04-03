#include <ECS/component_registry.hpp>

#pragma once


namespace SupDef {

    struct PlayerComponent : public Component {
        EntityID teamID = NO_ENTITY;
        bool isNeutral = false;
        
        PlayerComponent() { addToRegistry(); }

        void addToRegistry() {
            ComponentRegistry::registerComponent(getTypeName(), []()
                { return std::make_unique<PlayerComponent>(); });
        }

        void to_json(json& j) const override {
            generic_to_json(j, this);
        }

        void from_json(const json& j) override {
            generic_from_json(j, this);
        }

        REFLECT_COMPONENT_BEGIN(PlayerComponent)
            REFLECT_FIELD(teamID)
            REFLECT_FIELD(isNeutral)
        REFLECT_COMPONENT_END()

        // void to_json(json& j) const override {
        //     j = json{{S_TEAM, teamID}, {S_IS_NEUTRAL, isNeutral}};
        // }
    
        // void from_json(const json& j) override {
        //     j.at(S_TEAM).get_to(teamID);
        //     j.at(S_IS_NEUTRAL).get_to(isNeutral);
        // }
    
        std::string getTypeName() const override {
            return SC_PLAYER;
        }
    };
    
}
