#include <ECS/include.hpp>
#include <Action/include.hpp>
#include <Game/path_finder.hpp>
#include <Game/collision_system.hpp>
#include <Game/collision_tracker.hpp>
#include <Game/command_tracker.hpp>
#include <Game/constants.hpp>
#include <App/Log/logger.hpp>

#pragma once


namespace SupDef {

    class Game : public Listener {
        public:
            std::vector<std::tuple<float, float>> pathTiles;

            Game();
            
            void initialize();
            
            void update(float deltaTime);

            void updateWorlds(float deltaTime);
            void updateWorld(float deltaTime, _EntWorld world);
            void updateMap(float deltaTime, _EntMapTiles map);

            Entity* addMap(AssetID mapAssetID);

            void handleTriggerCommand(const TriggerCommandEvent& event);
            void handleUpdateCommand(const UpdateCommandEvent& event);
            void handleCompleteCommand(EntityID entityID, EntityID techID, CommandID commandID, json data);
            void handleIncompleteCommand(Entity* command, EntityID entityID, EntityID techID, json j);
            bool isCommandComplete(Entity* command, json j);
            std::tuple<bool, Entity*, ActiveTechComponent*> isTechProperCommand(EntityID entityID, EntityID techID);
            
            json getFeedbackFromCheck(json& input);
            json getFeedbackFromCheck(json& input, std::string msg);
            Entity* getAssetFromCommand(CommandID commandID, json &data);

            bool checkRequirements(CommandID commandID, json &data, bool onAction);
            bool checkRequirements(RequirementComponent* reqComp, json &data, bool onAction);
            bool checkResourceReq(Entity* player, RequirementComponent* reqComp, json &data, bool onAction);

            void processActions();
            void processAction(Entity* command, EntityID entityID, EntityID playerID, json &data);
    
            void updateTempGoalMass(TilesComponent* tilesComp, _EntPosMovCols& comps);
            void updateTempGoal(TilesComponent* tilesComp, _EntPosMovCol comp);
            void updateTempGoal(TilesComponent* tilesComp, Entity* entity);

            void updatePositionMass(float deltaTime, TilesComponent* tilesComp, _EntPosMovCols& comps);
            void updatePosition(float deltaTime, TilesComponent* tilesComp, _EntPosMovCol comp);
            void updatePosition(float deltaTime, TilesComponent* tilesComp, Entity* entity);
            void passPositionToChildren(Entity* entity, PositionComponent* positionComponent);
            void passPositionToChildren(Entity* entity);
            void passPositionToChildren(EntityID entityID);

            CollisionPairs findCollisions(MapComponent* mapComponent, _EntPosCols& listA, _EntPosCols& listB, bool groupMode);
            void updateCollisions(MapComponent* mapComponent, _EntPosCols& listA, _EntPosCols& listB, CollisionGroup collisionGroup, bool groupMode);
            void updateCollisions(MapComponent* mapComponent, _EntPosCols& listA, _EntPosCols& listB, CollisionGroup collisionGroup);
            void updateCollisions(MapComponent* mapComponent, _EntPosCols& list, CollisionGroup collisionGroup);
            void removeResolvedCollisions(CollisionGroup collisionGroup);
            void removeResolvedCollisions();
            
            Entity* createEntityFromAsset(Entity* asset);
            Entity* createEntityFromAsset(AssetID assetID);
            Entity* createEntityFromAsset(AssetID assetID, EntityID parentID);
            Entity* createEntityFromAsset(AssetID assetID, EntityID parentID, EntityID ownerID);
            Entity* createEntityFromAsset(AssetID assetID, float x, float y);
            Entity* createEntityFromAsset(AssetID assetID, EntityID parentID, float x, float y);
            Entity* createEntityFromAsset(AssetID assetID, EntityID parentID, EntityID ownerID, float x, float y);

            void setInitialOwner(Entity* entity, EntityID ownerID);
            void setInitialOwner(EntityID entityID, EntityID ownerID);
            
            Entity* realizeVirtualEntity();
            void createVirtualEntityFromAsset(Entity* asset);
            void createVirtualEntityFromAsset(AssetID assetID);
            
            void processTechs();
            void processTechsForEntity(EntityID entityID, EntityIDs& techIDs);
            void assignTechs(_EntTechs& techs);
            Entity* getCommandFromActiveTech(Entity* tech);
            Entity* getCommandFromActiveTech(ActiveTechComponent* activeTechComp);

            EntityManager*   getEntityManager  () { return entityManager  .get(); }
            AssetManager*    getAssetManager   () { return assetManager   .get(); }
            TilesChecker*    getTilesChecker   () { return tilesChecker   .get(); }
            PathFinder*      getPathFinder     () { return pathFinder     .get(); }
            CollisionSystem* getCollisionSystem() { return collisionSystem.get(); }
            CommandTracker * getCommandTracker () { return commandTracker .get(); }
            void setGlobalDispatcher(EventDispatcher* globalDispatcher_) { globalDispatcher = globalDispatcher_; }
            void setActionQueue(ActionQueue* actionQueue_) { actionQueue = actionQueue_; }
            void setThisPlayer(EntityID playerID) { thisPlayer = playerID; }
            Entity* getThisPlayer() { return entityManager->getEntity(thisPlayer); }
            Entity* getVirtualEntity() { if (!virtualEntity) return nullptr; return virtualEntity.get(); }

            TechMap& getTechToAssignees() { return techToAssignees; }
            TechMap& getAssigneeToTechs() { return assigneeToTechs; }

            void to_json    (json& j) const;
            void from_json  (const json& j);
            void serialize  (const std::string& filename) const;
            void deserialize(const std::string& filename);
            void serialize  () const { serialize  (DEFAULT_FILENAME_SAVEGAME); }
            void deserialize()       { deserialize(DEFAULT_FILENAME_SAVEGAME); }

        private:
            EntityID thisPlayer = NO_ENTITY;
            UEntityManager    entityManager    = nullptr;
            UAssetManager     assetManager     = nullptr;
            UEventDispatcher  eventDispatcher  = nullptr;
            UTilesChecker     tilesChecker     = nullptr;
            UPathFinder       pathFinder       = nullptr;
            UCollisionSystem  collisionSystem  = nullptr;
            UCollisionTracker collisionTracker = nullptr;
            UCommandTracker   commandTracker   = nullptr;
            ActionQueue*      actionQueue      = nullptr;

            UEntity virtualEntity = nullptr;
            UEntity uniqueCommand = nullptr;

            TechMap techToAssignees;
            TechMap assigneeToTechs;

    };
    
    using UGame = std::unique_ptr<Game>;

}