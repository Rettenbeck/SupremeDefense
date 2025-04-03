#include <ECS/component_registry.hpp>

#pragma once


namespace SupDef {

    struct CommandComponent : public Component {
        bool isUnique = false;
        Cooldown cooldown = NO_COOLDOWN;
        
        CommandComponent() { addToRegistry(); }
        CommandComponent(bool unique) : isUnique(unique) { addToRegistry(); }
        CommandComponent(bool unique, Cooldown cooldown) : isUnique(unique), cooldown(cooldown) { addToRegistry(); }
        
        void addToRegistry() {
            ComponentRegistry::registerComponent(getTypeName(), []()
                { return std::make_unique<CommandComponent>(); });
        }

        void to_json(json& j) const override {
            generic_to_json(j, this);
        }

        void from_json(const json& j) override {
            generic_from_json(j, this);
        }

        REFLECT_COMPONENT_BEGIN(CommandComponent)
            REFLECT_FIELD(isUnique)
            REFLECT_FIELD(cooldown)
        REFLECT_COMPONENT_END()

        // void to_json(json& j) const override {
        //     if (isUnique) j[S_IS_UNIQUE] = isUnique;
        //     j[S_COOLDOWN] = cooldown;
        // }
    
        // void from_json(const json& j) override {
        //     if (j.contains(S_IS_UNIQUE)) isUnique = j.at(S_IS_UNIQUE).get<bool>();
        //     else isUnique = false;
        //     cooldown = j.at(S_COOLDOWN).get<Cooldown>();
        // }
    
        std::string getTypeName() const override {
            return SCA_COMMAND;
        }

        bool isAsset() const override { return true; }

    };
    
}
