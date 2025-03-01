#include <ECS/entity.hpp>
#include <EventDispatcher/include.hpp>

#pragma once


namespace SupDef {
    
    class EntityManager {
        private:
            EventDispatcher* eventDispatcher = nullptr;
            std::unordered_map<EntityID, UEntity> entities;
            std::unordered_map<EntityID, EntityID> parentMap;
            std::unordered_map<EntityID, std::vector<EntityID>> childrenMap;
            EntityID nextEntityID = 1;

        public:
            EntityManager(EventDispatcher* eventDispatcher_) : eventDispatcher(eventDispatcher_) {}
            EntityManager() {}

            Entity* createEntity() {
                EntityID id = nextEntityID++;
                auto newEntity = std::make_unique<Entity>(id);
                auto rawPtr = newEntity.get();
                entities[id] = std::move(newEntity);
                return rawPtr;
            }

            EntityID createEntityID() {
                EntityID id = nextEntityID++;
                entities[id] = std::make_unique<Entity>(id);
                return id;
            }

            void setParent(EntityID childID, EntityID parentID) {
                parentMap[childID] = parentID;
                childrenMap[parentID].push_back(childID);
            }

            Entity* createEntity(int parentID) {
                auto rawPtr = createEntity();
                setParent(rawPtr->id, parentID);
                return rawPtr;
            }

            EntityID createEntityID(int parentID) {
                auto id = createEntityID();
                setParent(id, parentID);
                return id;
            }

            Entity* getEntity(EntityID id) const {
                if(id == NO_ENTITY) return nullptr;
                auto it = entities.find(id);
                return (it != entities.end()) ? it->second.get() : nullptr;
            }

            PEntities getEntities(EntityIDs group) const {
                PEntities result;
                for (auto id : group) {
                    auto ptr = getEntity(id);
                    if (ptr) result.push_back(ptr);
                }
                return result;
            }

            // PEntities getEntities() {
            //     PEntities result;
            //     for (const auto& entity : entities) {
            //         result.push_back(entity.second.get());
            //     }
            //     return result;
            // }

            EntityID getParentID(EntityID entityID) const {
                auto it = parentMap.find(entityID);
                return (it != parentMap.end()) ? it->second : NO_ENTITY;
            }

            Entity* getParent(EntityID entityID) const {
                auto parentID = getParentID(entityID);
                return getEntity(parentID);
            }

            EntityIDs getChildrenID(EntityID entityID) const {
                auto it = childrenMap.find(entityID);
                return (it != childrenMap.end()) ? it->second : EntityIDs{};
            }

            PEntities getChildren(EntityID entityID) const {
                PEntities result;
                auto children = getChildrenID(entityID);
                for(auto c : children) {
                    result.push_back(getEntity(c));
                }
                return result;
            }

            template <typename... ComponentTypes>
            PEntities extractEntities(const std::vector<std::tuple<Entity*, ComponentTypes*...>>& tuples) {
                PEntities entities;
                entities.reserve(tuples.size());
                for (const auto& tuple : tuples) {
                    entities.push_back(std::get<0>(tuple));
                }
                return entities;
            }

            template <typename... ComponentTypes>
            std::vector<std::tuple<Entity*, ComponentTypes*...>> getEntitiesWithComponents() {
                std::vector<std::tuple<Entity*, ComponentTypes*...>> result;
                for (const auto& entity : entities) {
                    if ((entity.second->getComponent<ComponentTypes>() && ...)) {
                        result.emplace_back(entity.second.get(), entity.second->getComponent<ComponentTypes>()...);
                    }
                }
                return result;
            }

            template <typename... ComponentTypes>
            std::vector<std::tuple<Entity*, ComponentTypes*...>> getEntitiesWithComponents(PEntities group) {
                std::vector<std::tuple<Entity*, ComponentTypes*...>> result;
                for (const auto& entity : group) {
                    if ((entity->getComponent<ComponentTypes>() && ...)) {
                        result.emplace_back(entity, entity->getComponent<ComponentTypes>()...);
                    }
                }
                return result;
            }

            template <typename... ComponentTypes>
            std::vector<std::tuple<Entity*, ComponentTypes*...>> getEntitiesWithComponents(EntityIDs group) {
                return getEntitiesWithComponents<ComponentTypes...>(getEntities(group));
            }

            template <typename... ComponentTypes>
            std::vector<std::tuple<Entity*, ComponentTypes*...>> getEntitiesWithComponents(EntityID parentID) {
                return getEntitiesWithComponents<ComponentTypes...>(getChildren(parentID));
            }

            void removeEntityFlat(EntityID entityID) {
                EntityID parentID = getParentID(entityID);
                if (parentID != NO_ENTITY) {
                    auto& siblings = childrenMap[parentID];
                    siblings.erase(std::remove(siblings.begin(), siblings.end(), entityID), siblings.end());
                }

                parentMap.erase(entityID);
                childrenMap.erase(entityID);

                eventDispatcher->dispatch<EntityDestroyedEvent>(entityID);
                entities.erase(entityID);
            }

            void removeEntity(EntityID entityID) {
                auto children = getChildrenID(entityID);
                for (auto childID : children) {
                    removeEntity(childID);
                }
                removeEntityFlat(entityID);
            }

            void to_json(json& j) const {
                for (const auto& [id, entity] : entities) {
                    json entityJson;
                    entity->to_json(entityJson);
                    if (parentMap.find(id) != parentMap.end()) {
                        entityJson[S_PARENT] = parentMap.at(id);
                    }
                    j[S_ENTITIES].push_back(entityJson);
                }
                j[S_NEXT_ENTITY_ID] = nextEntityID;
            }
        
            void from_json(const json& j) {
                entities.clear();
                parentMap.clear();
                childrenMap.clear();
            
                std::unordered_map<EntityID, EntityID> tempParentMap;
                std::unordered_map<EntityID, std::vector<EntityID>> tempChildrenMap;

                for (const auto& entityJson : j[S_ENTITIES]) {
                    EntityID id = entityJson[S_ID].get<EntityID>();
                    auto entity = std::make_unique<Entity>(id);
                    entity->from_json(entityJson);
                    entities[id] = std::move(entity);
                    
                    if (entityJson.contains(S_PARENT)) {
                        tempParentMap[id] = entityJson[S_PARENT].get<EntityID>();
                    }
                }
                
                for (const auto& [childID, parentID] : tempParentMap) {
                    setParent(childID, parentID);
                }

                nextEntityID = j[S_NEXT_ENTITY_ID];
            }

    };

    using UEntityManager = std::unique_ptr<EntityManager>;

}
