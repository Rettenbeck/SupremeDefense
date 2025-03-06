#include <ECS/include.hpp>
#include <Action/include.hpp>
#include <Game/path_finder.hpp>
#include <Game/collision_system.hpp>
#include <Game/collision_tracker.hpp>
#include <Game/command_processor.hpp>
#include <Game/constants.hpp>
#include <App/Log/logger.hpp>

#pragma once


namespace SupDef {

    using _EntWorld = std::tuple<Entity*, WorldComponent*>;
    using _EntMapTiles = std::tuple<Entity*, MapComponent*, TilesComponent*>;
    using _EntPosMovCol  = std::tuple<Entity*, PositionComponent*, MovementComponent*, CollisionComponent*>;
    using _EntPosMovCols = std::vector<_EntPosMovCol>;
    using _EntPosCol  = std::tuple<Entity*, PositionComponent*, CollisionComponent*>;
    using _EntPosCols = std::vector<_EntPosCol>;
    
    class Game : public Listener {
        private:
            EntityID thisPlayer = NO_ENTITY;
            UEntityManager    entityManager    = nullptr;
            UAssetManager     assetManager     = nullptr;
            UComProcessor     comProcessor     = nullptr;
            UEventDispatcher  eventDispatcher  = nullptr;
            UTilesChecker     tilesChecker     = nullptr;
            UPathFinder       pathFinder       = nullptr;
            UCollisionSystem  collisionSystem  = nullptr;
            UCollisionTracker collisionTracker = nullptr;
            ActionQueue*      actionQueue      = nullptr;
            UEntity virtualEntity = nullptr;
            UEntity uniqueCommand = nullptr;

        public:
            Game();
            
            void initialize();
            
            void update(float deltaTime);

            void updateWorld(float deltaTime, _EntWorld world);
            void updateMap(float deltaTime, _EntMapTiles map);

            Entity* addMap(AssetID mapAssetID);

            void updateCommands();
            void processDirectCommands();
            void handleStartCommand(Entity* command);
            void handleUpdateCommand(Entity* command);
            void handleConfirmCommand(Entity* command);
            
            Entity* getAssetFromCommand(CommandID commandID, json &data);

            void processActions();
            void processAction(Entity* command, EntityID playerID, json &data);

            json getFeedbackFromCheck(json& input);
            json getFeedbackFromCheck(json& input, std::string msg);
            bool checkRequirements(CommandID commandID, json &data, CommandStatus status, bool onAction);
            bool checkRequirements(RequirementComponent* reqComp, json &data, CommandStatus status, bool onAction);
            bool checkResourceReq(Entity* player, RequirementComponent* reqComp, json &data,
                CommandStatus status, bool onAction);

            void updateTempGoalMass(TilesComponent* tilesComp, _EntPosMovCols& comps);
            void updateTempGoal(TilesComponent* tilesComp, _EntPosMovCol comp);
            void updateTempGoal(TilesComponent* tilesComp, Entity* entity);

            void updatePositionMass(float deltaTime, TilesComponent* tilesComp, _EntPosMovCols& comps);
            void updatePosition(float deltaTime, TilesComponent* tilesComp, _EntPosMovCol comp);
            void updatePosition(float deltaTime, TilesComponent* tilesComp, Entity* entity);

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
            
            EntityManager*   getEntityManager  () { return entityManager  .get(); }
            AssetManager*    getAssetManager   () { return assetManager   .get(); }
            CommandProcessor*getComProcessor   () { return comProcessor   .get(); }
            TilesChecker*    getTilesChecker   () { return tilesChecker   .get(); }
            PathFinder*      getPathFinder     () { return pathFinder     .get(); }
            CollisionSystem* getCollisionSystem() { return collisionSystem.get(); }
            void setGlobalDispatcher(EventDispatcher* globalDispatcher_) { globalDispatcher = globalDispatcher_; }
            void setActionQueue(ActionQueue* actionQueue_) { actionQueue = actionQueue_; }
            void setThisPlayer(EntityID playerID) { thisPlayer = playerID; }
            Entity* getThisPlayer() { return entityManager->getEntity(thisPlayer); }

            void to_json    (json& j) const;
            void from_json  (const json& j);
            void serialize  (const std::string& filename) const;
            void deserialize(const std::string& filename);
            void serialize  () const { serialize  (DEFAULT_FILENAME_SAVEGAME); }
            void deserialize()       { deserialize(DEFAULT_FILENAME_SAVEGAME); }

    };
    
    using UGame = std::unique_ptr<Game>;

}