#pragma once

#include <ECS/include.hpp>
#include <Action/include.hpp>
#include <Game/path_finder.hpp>
#include <Game/collision_system.hpp>
#include <Game/collision_tracker.hpp>
#include <Game/command_tracker.hpp>
#include <Game/constants.hpp>
#include <App/Log/logger.hpp>


namespace SupDef {

    class Game : public Listener {
        public:
            std::vector<std::tuple<float, float>> pathTiles;
            EntityID otherPlayer = NO_ENTITY;
            std::string toPrint = "";

            Game();
            
            void initialize();
            
            void update(float deltaTime);



            // ### GENERAL FUNCTIONS ###################################################### //
            void updateWorlds(float deltaTime);
            void updateWorld(float deltaTime, _EntWorld world);
            void updateMap(float deltaTime, _EntMapTiles map);

            Entity* addMap(AssetID mapAssetID);
            EntityID getMapOfEntity(EntityID entityID);
            void generatePlayerList();

            


            // ### STARTUP FUNCTIONS ###################################################### //
            void startWorld(AssetID worldID, PlayerMapExt playerMapExt_, int thisPlayer_);
            Entity* createPlayer(AssetID playerID, AssetID spawnID, EntityID mapID);
            EntityID getMapIDBySpawn(AssetID spawnAssetID);


            

            // ### COMMAND MANAGEMENT ##################################################### //
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
            


            // ### MOVEMENT HANDLING ###################################################### //
            void updateTempGoalMass(TilesComponent* tilesComp, _EntPosMovCols& comps);
            void updateTempGoal(TilesComponent* tilesComp, _EntPosMovCol comp);
            void updateTempGoal(TilesComponent* tilesComp, Entity* entity);

            void updatePositionMass(float deltaTime, TilesComponent* tilesComp, _EntPosMovCols& comps);
            void updatePosition(float deltaTime, TilesComponent* tilesComp, _EntPosMovCol comp);
            void updatePosition(float deltaTime, TilesComponent* tilesComp, Entity* entity);
            void updatePositionDirected(float deltaTime, TilesComponent* tilesComp, _EntPosMovCol comp);
            void passPositionToChildren(Entity* entity, PositionComponent* positionComponent);
            void passPositionToChildren(Entity* entity);
            void passPositionToChildren(EntityID entityID);
            void setNewPositionByDifference(PositionComponent* pos, float dx, float dy);
            void setNewPosition(PositionComponent* pos, float x, float y);
            void setNewCenteredPosition(Entity* entity, PositionComponent* pos, float x, float y);
            void setNewCenteredPosition(Entity* entity, float x, float y);
            void setInitialPosition(Entity* entity, InitPositionComponent* initComp);
            

            
            // ### COLLISION HANDLING ##################################################### //
            void determineCollisions();
            void determineCollisionsInfluence();
            void determineCollisionsWeapons();
            void determineCollisionsProjectiles();
            void determineCollisionsEnemyGoals();

            template <typename... T>
            void determineCollisionsGeneric(CollisionGroup collisionGroup, bool influenceMode = false);

            template<typename... A, typename... B>
            void determineCollisionsGeneric(std::tuple<A...> a, std::tuple<B...> b, CollisionGroup collisionGroup, bool influenceMode = false);

            void processCollisions(PEntities group, CollisionGroup collisionGroup);
            void processCollisions(PEntities group1, PEntities group2, CollisionGroup collisionGroup);
            _Map_EntPosCols convertEntitiesForCollision(PEntities group);
            CollisionGroup buildCollisionGroup(CollisionGroup collisionGroup, EntityID mapID);
            CollisionPairs findCollisions(MapComponent* mapComponent, _EntPosCols& listA, _EntPosCols& listB, bool groupMode);
            void updateCollisions(MapComponent* mapComponent, _EntPosCols& listA, _EntPosCols& listB, CollisionGroup collisionGroup, bool groupMode);
            void updateCollisions(MapComponent* mapComponent, _EntPosCols& listA, _EntPosCols& listB, CollisionGroup collisionGroup);
            void updateCollisions(MapComponent* mapComponent, _EntPosCols& list, CollisionGroup collisionGroup);
            void removeResolvedCollisions(CollisionGroup collisionGroup);
            void removeResolvedCollisions();
            VF2 getCenterOfEntity(Entity* entity, PositionComponent* pos, CollisionComponent* col);
            VF2 getCenterOfEntity(Entity* entity, PositionComponent* pos);
            VF2 getCenterOfEntity(Entity* entity);
            EntityIDs findCollisionPartners(EntityID entityID, CollisionGroup collisionGroup);
            PCollisionInfos findCollisionsOfEntity(EntityID entityID, CollisionGroup collisionGroup);
            

            
            // ### ENTITY CREATION ######################################################## //
            Entity* createEntityFromAsset(Entity* asset);
            Entity* createEntityFromAsset(AssetID assetID);
            Entity* createEntityFromAsset(AssetID assetID, EntityID parentID);
            Entity* createEntityFromAsset(AssetID assetID, EntityID parentID, EntityID ownerID);
            Entity* createEntityFromAsset(AssetID assetID, float x, float y);
            Entity* createEntityFromAsset(AssetID assetID, EntityID parentID, float x, float y);
            Entity* createEntityFromAsset(AssetID assetID, EntityID parentID, EntityID ownerID, float x, float y);
            void setParentOfNewEntity(AssetID assetID, Entity* entity, EntityID parentID);

            void setInitialOwner(Entity* entity, EntityID ownerID);
            void setInitialOwner(EntityID entityID, EntityID ownerID);
            
            Entity* realizeVirtualEntity();
            void createVirtualEntityFromAsset(Entity* asset);
            void createVirtualEntityFromAsset(AssetID assetID);
            

            
            // ### TECH MANAGEMENT ######################################################## //
            void processTechs();
            void processTechsForEntity(EntityID entityID, TechAssignments& techAssignments);
            void assignTechs(_EntTechs& techs);
            void assignTech(Entity* entity, TechComponent* tech);
            void filterTech(Entity* entity, TechComponent* tech);
            void filterTechByComponents(Entity* entity, TechComponent* tech);
            void assignTechByList(Entity* entity, TechComponent* tech);
            void distributeTechAssignment(_EntTechs& techList);
            void createGiftedTech(Entity* gifted, Entity* gifter, GiftTechComponent* giftComp);
            void removeGiftedTech(Entity* gifted, Entity* gifter, GiftTechComponent* giftComp);
            void removeObsoleteGiftedTechs();

            EntityIDsTriple compareVectors(const EntityIDs& vec1, const EntityIDs& vec2);
            Entity* getCommandFromActiveTech(Entity* tech);
            Entity* getCommandFromActiveTech(ActiveTechComponent* activeTechComp);
            

            
            // ### WEAPON HANDLING ######################################################## //
            void processWeapons();
            void processWeapon(Entity* entity, PositionComponent* pos, CollisionComponent* col, WeaponComponent* wpn);
            void clearWeapon(WeaponComponent* wpn);
            void determineTargetsForWeapon(Entity* entity, WeaponComponent* wpn);
            void sortTargetsForWeapon(Entity* entity, WeaponComponent* wpn);
            void shootWeapon(Entity* entity, PositionComponent* pos, CollisionComponent* col, WeaponComponent* wpn);
            void shootWeaponNormal(Entity* entity, Entity* target, EntityID mapID, Entity* owner,
                PositionComponent* pos, CollisionComponent* col, WeaponComponent* wpn);
            

            
            // ### PROJECTILE HANDLING #################################################### //
            void processProjectiles();
            void processProjectile(EntityID entityID, PositionComponent* pos, MovementComponent* mov,
                CollisionComponent* col, ProjectileComponent* pro);
            void processProjectileCollisions(EntityID projectileID, ProjectileComponent* pro);
            void processProjectileCollision(EntityID projectileID, ProjectileComponent* pro, Entity* other, CollisionInfo* collision);
            void removeProjectile(EntityID projectileID);
            void targetDestroyed(EntityID targetID);
            void removeOffMapProjectile(EntityID entityID, PositionComponent* pos, CollisionComponent* col);
            

            
            // ### PLAYER HANDLING ######################################################## //
            Entity* getPlayer(Entity* entity);
            Entity* getPlayer(EntityID entityID);
            Relation getRelation(Entity* entity1, Entity* entity2);
            Relation getRelation(EntityID entityID1, EntityID entityID2);
            void retrieveChildrenRecursive(EntityID entityID, PEntities& list);
            PEntities retrieveChildrenRecursive(EntityID entityID);
            

            
            // ### ENEMIES HANDLING ####################################################### //
            void processEnemies();
            void setEnemyMovementTowardsGoal(Entity* entity, MovementComponent* mov, EnemyGoalSeekerComponent *skr);
            void checkSpawner(Entity* entity, PositionComponent* pos, EnemySpawnerComponent* spw);
            void spawnEnemy(PositionComponent* pos, WaveSegment* waveSegment, EntityID goalID, EntityID mapID);
            

            
            // ### REFERENCE METHODS ###################################################### //
            EntityManager*    getEntityManager   () { return entityManager   .get(); }
            AssetManager*     getAssetManager    () { return assetManager    .get(); }
            TilesChecker*     getTilesChecker    () { return tilesChecker    .get(); }
            PathFinder*       getPathFinder      () { return pathFinder      .get(); }
            CollisionSystem*  getCollisionSystem () { return collisionSystem .get(); }
            CollisionTracker* getCollisionTracker() { return collisionTracker.get(); }
            CommandTracker *  getCommandTracker  () { return commandTracker  .get(); }
            void setGlobalDispatcher(EventDispatcher* globalDispatcher_) { globalDispatcher = globalDispatcher_; }
            void setActionQueue(ActionQueue* actionQueue_) { actionQueue = actionQueue_; }
            void setThisPlayer(EntityID playerID) { thisPlayer = playerID; }
            void setAssetManager(UAssetManager assetManager_) { assetManager = std::move(assetManager_); }
            Entity* getThisPlayer() { return entityManager->getEntity(thisPlayer); }
            Entity* getVirtualEntity() { if (!virtualEntity) return nullptr; return virtualEntity.get(); }

            TechMap& getTechToAssignees() { return techToAssignees; }
            TechMap& getAssigneeToTechs() { return assigneeToTechs; }
            void setFrameCount(long frameCount_) { frameCount = frameCount_; }
            

            
            // ### SERIALIZATION ########################################################## //
            void to_json    (json& j) const;
            void from_json  (const json& j);
            void serialize  (const std::string& filename) const;
            void deserialize(const std::string& filename);
            void serialize  () const { serialize  (DEFAULT_FILENAME_SAVEGAME); }
            void deserialize()       { deserialize(DEFAULT_FILENAME_SAVEGAME); }

        private:
            EntityID thisPlayer = NO_ENTITY;
            PlayerMapExt playerMapExt;
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

            long frameCount = 0;

    };
    
    using UGame = std::unique_ptr<Game>;

}