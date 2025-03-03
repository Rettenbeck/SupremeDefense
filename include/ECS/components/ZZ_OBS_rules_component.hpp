#include <ECS/component_registry.hpp>
#include <Game/rules.hpp>

#pragma once


namespace SupDef {

    class RulesComponent : public Component {
        URules rules = nullptr;
    
        public:
            RulesComponent() {
                rules = std::make_unique<Rules>();
                addToRegistry();
            }

            explicit RulesComponent(URules rules_) : rules(std::move(rules_)) {
                addToRegistry();
            }
            
            void addToRegistry() {
                ComponentRegistry::registerComponent(getTypeName(), []()
                    { return std::make_unique<RulesComponent>(); });
            }
    
            Rules* getRules() {
                return rules.get();
            }

            // template <typename T>
            // std::optional<T> getRule(const std::string& key) const {
            //     return rules->get<T>(key);
            // }
        
            void to_json(json& j) const override {
                rules->to_json(j[S_RULES]);
            }
        
            void from_json(const json& j) override {
                rules->from_json(j[S_RULES]);
            }

            std::string getTypeName() const override {
                return SC_RULES;
            }
            
    };

}
