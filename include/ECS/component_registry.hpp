#include <ECS/component.hpp>

#pragma once


namespace SupDef {
    
    class ComponentRegistry {

        using CreateComponentFunc    = std::function<std::unique_ptr<Component>()>;
        using CreateComponentFuncMap = std::unordered_map<std::string, CreateComponentFunc>;

        static CreateComponentFuncMap& registry() {
            static CreateComponentFuncMap instance;
            return instance;
        }
        
        public:
            static void registerComponent(const std::string& typeName, CreateComponentFunc func) {
                registry()[typeName] = func;
            }
        
            static std::unique_ptr<Component> createComponent(const std::string& typeName) {
                auto it = registry().find(typeName);
                if (it != registry().end()) {
                    return it->second();
                }
                return nullptr;
            }
            
    };
    
}
