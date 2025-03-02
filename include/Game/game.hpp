#include <ECS/include.hpp>
#include <Tech/include.hpp>
#include <Game/path_finder.hpp>
#include <Game/collision_system.hpp>
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
        EntityID thisPlayer = NO_ENTITY;
        UEntityManager   entityManager   = nullptr;
        UAssetManager    assetManager    = nullptr;
        UTechManager     techManager     = nullptr;
        UComProcessor    comProcessor    = nullptr;
        UEventDispatcher eventDispatcher = nullptr;
        UTilesChecker    tilesChecker    = nullptr;
        UPathFinder      pathFinder      = nullptr;
        UCollisionSystem collisionSystem = nullptr;

        public:
            Game();
            
            void initialize();
            
            void update(float deltaTime);

            void updateWorld(float deltaTime, _EntWorld world);
            void updateMap(float deltaTime, _EntMapTiles map);

            Entity* addMap(AssetID mapAssetID);

            void updateCommands();
            bool checkRequirements(RequirementComponent* reqComp, CommandStatus status);
            bool checkRequirements(CommandID commandID, CommandStatus status);
            bool checkResourceRequirements(Entity* player, RequirementComponent* reqComp, CommandStatus status);

            void updateTempGoalMass(TilesComponent* tilesComp, _EntPosMovCols& comps);
            void updateTempGoal(TilesComponent* tilesComp, _EntPosMovCol comp);
            void updateTempGoal(TilesComponent* tilesComp, Entity* entity);

            void updatePositionMass(float deltaTime, TilesComponent* tilesComp, _EntPosMovCols& comps);
            void updatePosition(float deltaTime, TilesComponent* tilesComp, _EntPosMovCol comp);
            void updatePosition(float deltaTime, TilesComponent* tilesComp, Entity* entity);

            CollisionPairs findCollisions(MapComponent* mapComponent, _EntPosCols& listA, _EntPosCols& listB, bool groupMode);
            CollisionPairs findCollisions(MapComponent* mapComponent, _EntPosCols& listA, _EntPosCols& listB);
            CollisionPairs findCollisions(MapComponent* mapComponent, _EntPosCols& list);
            
            Entity* createEntityFromAsset(Entity* asset);
            Entity* createEntityFromAsset(AssetID assetID);
            Entity* createEntityFromAsset(AssetID assetID, EntityID parentID);
            Entity* createEntityFromAsset(AssetID assetID, float x, float y);
            Entity* createEntityFromAsset(AssetID assetID, EntityID parentID, float x, float y);
            
            EntityManager*   getEntityManager  () { return entityManager  .get(); }
            AssetManager*    getAssetManager   () { return assetManager   .get(); }
            TechManager*     getTechManager    () { return techManager    .get(); }
            CommandProcessor*getComProcessor   () { return comProcessor   .get(); }
            TilesChecker*    getTilesChecker   () { return tilesChecker   .get(); }
            PathFinder*      getPathFinder     () { return pathFinder     .get(); }
            CollisionSystem* getCollisionSystem() { return collisionSystem.get(); }
            void setGlobalDispatcher(EventDispatcher* globalDispatcher_) { globalDispatcher = globalDispatcher_; }
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