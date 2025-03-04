#include <ECS/component_registry.hpp>

#pragma once


namespace SupDef {

    struct CommandComponent : public Component {
        bool isUnique = false;
        
        CommandComponent() { addToRegistry(); }
        CommandComponent(bool unique) : isUnique(unique) { addToRegistry(); }
        // CommandComponent(CommandType commandType) : commandType(commandType) { addToRegistry(); }
        
        void addToRegistry() {
            ComponentRegistry::registerComponent(getTypeName(), []()
                { return std::make_unique<CommandComponent>(); });
        }

        void to_json(json& j) const override {
            if (isUnique) j[S_IS_UNIQUE] = isUnique;
        }
    
        void from_json(const json& j) override {
            if (j.contains(S_IS_UNIQUE)) isUnique = j.at(S_IS_UNIQUE).get<bool>();
            else isUnique = false;
        }
    
        std::string getTypeName() const override {
            return SCA_COMMAND;
        }

        bool isAssetOnly() const override { return true; }

    };
    
}
