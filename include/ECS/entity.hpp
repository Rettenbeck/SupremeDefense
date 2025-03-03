#include <ECS/components.hpp>

#pragma once

// #ifndef Included_Entity_HPP

// #define Included_Entity_HPP

namespace SupDef {
    
    class Entity {
        protected:
            std::unordered_map<std::type_index, UComponent> components;

        public:
            EntityID id = NO_ENTITY;
            AssetID assetID = NO_ASSET;
            bool isDead = false;

            Entity(EntityID id) : id(id) {}
            Entity(EntityID id, AssetID assetID) : id(id), assetID(assetID) {}

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
            
            std::unordered_map<std::type_index, UComponent>& getComponents() {
                return components;
            }

            template <typename T>
            bool hasComponent() const {
                return components.find(typeid(T)) != components.end();
            }

            void to_json(json& j) const {
                if (id != NO_ENTITY) j[S_ID] = id;
                j[S_ASSET_ID] = assetID;
                for (const auto& [type, component] : components) {
                    json componentJson;
                    component->to_json(componentJson);
                    componentJson[S_TYPE] = component->getTypeName();
                    j[S_COMPONENTS].push_back(componentJson);
                }
            }
        
            void to_json_skip_assets(json& j) const {
                if (id != NO_ENTITY) j[S_ID] = id;
                j[S_ASSET_ID] = assetID;
                for (const auto& [type, component] : components) {
                    if (component->isAssetOnly()) continue;
                    json componentJson;
                    component->to_json(componentJson);
                    componentJson[S_TYPE] = component->getTypeName();
                    j[S_COMPONENTS].push_back(componentJson);
                }
            }
        
            void from_json(const json& j) {
                if (j.contains(S_ID)) id = j.at(S_ID).get<EntityID>();
                else id = NO_ENTITY;

                assetID = j.at(S_ASSET_ID).get<AssetID>();
                if (j.contains(S_COMPONENTS) && j[S_COMPONENTS].is_array()) {
                    for (const auto& componentJson : j[S_COMPONENTS]) {
                        std::string typeName = componentJson.at(S_TYPE).get<std::string>();
                        auto component = ComponentRegistry::createComponent(typeName);
                        assert(component);
                        component->from_json(componentJson);
                        components[typeid(*component)] = std::move(component);
                    }
                }
            }

    };

    using UEntity = std::unique_ptr<Entity>;
    using PEntities = std::vector<Entity*>;

}

// #endif
