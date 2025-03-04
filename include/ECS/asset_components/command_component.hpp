#include <ECS/component_registry.hpp>

#pragma once


namespace SupDef {

    struct CommandComponent : public Component {
        CommandType commandType;
        
        CommandComponent() { addToRegistry(); }
        CommandComponent(CommandType commandType) : commandType(commandType) { addToRegistry(); }
        
        void addToRegistry() {
            ComponentRegistry::registerComponent(getTypeName(), []()
                { return std::make_unique<CommandComponent>(); });
        }

        void to_json(json& j) const override {
            j = json{{S_COMMAND_TYPE, commandType}};
        }
    
        void from_json(const json& j) override {
            j.at(S_COMMAND_TYPE).get_to(commandType);
        }
    
        std::string getTypeName() const override {
            return SCA_COMMAND;
        }

        bool isAssetOnly() const override { return true; }

    };
    
}
