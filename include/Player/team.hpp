#include <Player/constants.hpp>
#include <ECS/components.hpp>

#pragma once


namespace SupDef {

    struct Team {
        TeamID id = NO_TEAM;
        PlayerIDs players;
        std::unordered_map<std::type_index, UComponent> components;
        
        Team(TeamID id) : id(id) {}

        template <typename T, typename... Args>
        T* addComponent(Args&&... args) {
            auto component = std::make_unique<T>(std::forward<Args>(args)...);
            return addComponentDirect<T>(std::move(component));
        }

        template <typename T>
        T* addComponentDirect(UComponent component) {
            T* rawPtr = dynamic_cast<T*>(component.get());
            components[typeid(T)] = std::move(component);
            return rawPtr;
        }

        template <typename T>
        T* getComponent() {
            auto it = components.find(typeid(T));
            return (it != components.end()) ? static_cast<T*>(it->second.get()) : nullptr;
        }

        template <typename T>
        bool hasComponent() const {
            return components.find(typeid(T)) != components.end();
        }

        void to_json(json& j) const {
            j[S_ID] = id;
            j[SP_PLAYERS] = players;
            for (const auto& [type, component] : components) {
                json componentJson;
                component->to_json(componentJson);
                componentJson[S_TYPE] = component->getTypeName();
                j[S_COMPONENTS].push_back(componentJson);
            }
        }
    
        void from_json(const json& j) {
            id = j.at(S_ID).get<TeamID>();
            players = j.at(SP_PLAYERS).get<PlayerIDs>();
            if (j.contains(S_COMPONENTS) && j[S_COMPONENTS].is_array()) {
                for (const auto& componentJson : j[S_COMPONENTS]) {
                    std::string typeName = componentJson.at(S_TYPE).get<std::string>();
                    auto component = ComponentRegistry::createComponent(typeName);
                    if (component) {
                        component->from_json(componentJson);
                        components[typeid(*component)] = std::move(component);
                    } else {
                        assert(false);
                    }
                }
            }
        }

    };

    using UTeam = std::unique_ptr<Team>;
    using Teams = std::vector<UTeam>;

}
