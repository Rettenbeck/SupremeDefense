#include <Player/constants.hpp>
#include <ECS/components.hpp>

#pragma once


namespace SupDef {

    struct Player {
        PlayerID id = NO_PLAYER;
        TeamID teamID = NO_TEAM;
        std::unordered_map<std::type_index, UComponent> components;
        
        Player(PlayerID id) : id(id) {}
        Player(PlayerID id, TeamID teamID) : id(id), teamID(teamID) {}

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
            j[SP_TEAM] = teamID;
            for (const auto& [type, component] : components) {
                json componentJson;
                component->to_json(componentJson);
                componentJson[S_TYPE] = component->getTypeName();
                j[S_COMPONENTS].push_back(componentJson);
            }
        }
    
        void from_json(const json& j) {
            id = j.at(S_ID).get<PlayerID>();
            teamID = j.at(SP_TEAM).get<TeamID>();
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

    using UPlayer = std::unique_ptr<Player>;
    using Players = std::vector<UPlayer>;

}
