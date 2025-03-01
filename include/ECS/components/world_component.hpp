#include <ECS/component_registry.hpp>

#pragma once


namespace SupDef {

    struct WorldComponent : public Component {
        EntityIDs maps;
        EntityIDs players;
        
        WorldComponent() { addToRegistry(); }
        
        void addToRegistry() {
            ComponentRegistry::registerComponent(getTypeName(), []()
                { return std::make_unique<WorldComponent>(); });
        }

        void to_json(json& j) const override {
            j = json{{S_MAPS, maps}, {S_PLAYERS, players}};
        }
    
        void from_json(const json& j) override {
            j.at(S_MAPS).get_to(maps);
            j.at(S_PLAYERS).get_to(players);
        }
    
        std::string getTypeName() const override {
            return SC_WORLD;
        }
        
    };

}
