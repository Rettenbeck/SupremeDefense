#include <ECS/entity.hpp>
//#include <ECS/asset_components/asset_component.hpp>
#include <EventDispatcher/include.hpp>

#pragma once


namespace SupDef {
    
    class AssetManager {
        public:
            EventDispatcher* eventDispatcher = nullptr;
            std::unordered_map<AssetID, UEntity> assets;

            AssetManager(EventDispatcher* eventDispatcher_) : eventDispatcher(eventDispatcher_) {}
            AssetManager() {}

            Entity* createAsset(AssetID id) {
                auto newEntity = std::make_unique<Entity>(NO_ENTITY, id);
                auto rawPtr = newEntity.get();
                assets[id] = std::move(newEntity);
                return rawPtr;
            }

            Entity* getAsset(AssetID id) const {
                auto it = assets.find(id);
                return (it != assets.end()) ? it->second.get() : nullptr;
            }

            Entity* getAssetStrict(AssetID id) const {
                auto ptr = getAsset(id);
                assert(ptr);
                return ptr;
            }

            template <typename... ComponentTypes>
            PEntities extractAssets(const std::vector<std::tuple<Entity*, ComponentTypes*...>>& tuples) {
                PEntities entities;
                entities.reserve(tuples.size());
                for (const auto& tuple : tuples) {
                    entities.push_back(std::get<0>(tuple));
                }
                return entities;
            }

            template <typename... ComponentTypes>
            std::vector<std::tuple<Entity*, ComponentTypes*...>> getAssetsWithComponents() {
                std::vector<std::tuple<Entity*, ComponentTypes*...>> result;
                for (const auto& entity : assets) {
                    if ((entity.second->getComponent<ComponentTypes>() && ...)) {
                        result.emplace_back(entity.second.get(), entity.second->getComponent<ComponentTypes>()...);
                    }
                }
                return result;
            }

            template <typename... ComponentTypes>
            std::vector<std::tuple<Entity*, ComponentTypes*...>> getAssetsWithComponents(PEntities group) {
                std::vector<std::tuple<Entity*, ComponentTypes*...>> result;
                for (const auto& entity : group) {
                    if ((entity->getComponent<ComponentTypes>() && ...)) {
                        result.emplace_back(entity, entity->getComponent<ComponentTypes>()...);
                    }
                }
                return result;
            }

            void removeAsset(AssetID id) {
                assets.erase(id);
            }

            void to_json(json& j) const {
                for (const auto& [id, entity] : assets) {
                    json entityJson;
                    entity->to_json(entityJson);
                    j[S_ASSETS].push_back(entityJson);
                }
            }
        
            void from_json(const json& j) {
                assets.clear();
                for (const auto& entityJson : j[S_ASSETS]) {
                    auto entity = std::make_unique<Entity>(NO_ENTITY);
                    entity->from_json(entityJson);
                    assets[entity->assetID] = std::move(entity);
                }
            }

    };

    using UAssetManager = std::unique_ptr<AssetManager>;

}
