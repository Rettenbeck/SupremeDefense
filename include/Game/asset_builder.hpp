#pragma once

#include <Game/game.hpp>
#include <Game/BuildAssets/constants.hpp>


namespace SupDef {

    #define RETRIEVE_COMP(NAME, TYPE) \
    assert(asset); auto NAME = asset->retrieveComponent<TYPE>(); assert(NAME);

    struct AsData {
        AssetManager* am;
        AssetID assetID;
        std::string name_de, desc_de, name_en, desc_en;
        AsData() : am(nullptr), assetID(NO_ASSET), name_de(""), desc_de(""), name_en(""), desc_en("") {}
        AsData(AssetManager* am, AssetID assetID) : am(am), assetID(assetID),
            name_de(""), desc_de(""), name_en(""), desc_en("") {}
        AsData(AssetManager* am, AssetID assetID, std::string name_de, std::string desc_de) : am(am), assetID(assetID),
            name_de(name_de), desc_de(desc_de), name_en(""), desc_en("") {}
        AsData(AssetManager* am, AssetID assetID, std::string name_de, std::string desc_de,
            std::string name_en, std::string desc_en) : am(am), assetID(assetID),
            name_de(name_de), desc_de(desc_de), name_en(name_en), desc_en(desc_en) {}
    };

    struct TechData {
        TechComponent* techComponent = nullptr;
        float radius = 0.0;
        TechData() {}
        TechData(TechComponent* techComponent) : techComponent(techComponent) {}
        TechData(TechComponent* techComponent, float radius) : techComponent(techComponent), radius(radius) {}
    };

    struct ProjectileData {
        int multiHitCooldown = 0;
        bool homing = false;
        float radius = 1.0;
        float speed = 5.0;
        AssetIDs applyTechsOnHit;
        ProjectileData();
        ProjectileData(int multiHitCooldown, bool homing, float radius)
        : multiHitCooldown(multiHitCooldown), homing(homing), radius(radius) {}
        ProjectileData(int multiHitCooldown, bool homing, float radius, float speed)
        : multiHitCooldown(multiHitCooldown), homing(homing), radius(radius), speed(speed) {}
        ProjectileData(int multiHitCooldown, bool homing, float radius, AssetID applyTechOnHit)
        : multiHitCooldown(multiHitCooldown), homing(homing), radius(radius) {
            applyTechsOnHit.push_back(applyTechOnHit);
        }
        ProjectileData(int multiHitCooldown, bool homing, float radius, float speed, AssetID applyTechOnHit)
        : multiHitCooldown(multiHitCooldown), homing(homing), radius(radius), speed(speed) {
            applyTechsOnHit.push_back(applyTechOnHit);
        }
    };

    class AssetBuilder {
        public:

        static Entity* createEmptyAsset(AsData& data) {
            Entity* asset = nullptr;
            if (data.assetID.empty()) {
                asset = data.am->createAsset();
            } else {
                asset = data.am->createAsset(data.assetID);
            }
            assert(asset);
            auto textComp = asset->addComponent<TextComponent>();
            assert(textComp);
            if (!data.name_de.empty()) textComp->addText(SX_NAME, SX_LANG_DE, data.name_de);
            if (!data.desc_de.empty()) textComp->addText(SX_DESC, SX_LANG_DE, data.desc_de);
            if (!data.name_en.empty()) textComp->addText(SX_NAME, SX_LANG_EN, data.name_en);
            if (!data.desc_en.empty()) textComp->addText(SX_DESC, SX_LANG_EN, data.desc_en);
            return asset;
        }

        static void addResource(Entity* asset, ResourceID id, long amount = 0,
            float gatherRate = 0, long maxCapacity = INFINITE_CAPACITY) {
            //
            assert(asset); auto resComp = asset->retrieveComponent<ResourceComponent>(); assert(resComp);
            resComp->addResource(id, amount, gatherRate, maxCapacity);
        }

        static void addRequiredResource(Entity* asset, ResourceID id, long amount) {
            //
            assert(asset); auto req = asset->retrieveComponent<RequirementComponent>();
            req->add(std::make_unique<SupDef::Resource>(id, amount));
        }

        static void addInit(Entity* asset, AssetID assetID) {
            RETRIEVE_COMP(initComp, InitContainerComponent)
            initComp->add(assetID);
        }

        static void addInit(Entity* asset, AssetID assetID, float x, float y) {
            RETRIEVE_COMP(initComp, InitContainerComponent)
            initComp->add(assetID, x, y);
        }

        static void addInit(Entity* asset, AssetID assetID, VF2s positions) {
            RETRIEVE_COMP(initComp, InitContainerComponent)
            for (auto& [x, y] : positions) {
                initComp->add(assetID, x, y);
            }
        }

        static void addInit(Entity* asset, AssetIDs assetIDs) {
            for (auto& assetID : assetIDs) {
                addInit(asset, assetID);
            }
        }

        static void addRectangleShape(Entity* asset, float width, float height, bool calcDummyRadius = true) {
            RETRIEVE_COMP(colComp, CollisionComponent)
            if (calcDummyRadius) {
                colComp->dummyRadius = std::min(width, height);
            }
            colComp->addShape(std::make_unique<SupDef::RectangleShape>(width, height));
        }

        static void addDummyRadius(Entity* asset, float radius, bool isInfluence = false) {
            RETRIEVE_COMP(colComp, CollisionComponent)
            colComp->dummyRadius = radius;
            colComp->isInfluence = isInfluence;
        }

        static void addAnimation(Entity* asset, long totalFrames, int width, int height, int columnCount,
            bool dieAfterAnimation = false, MovementAnimation movementAnimation = MovementAnimation::AlwaysAnimate ) {
            //
            RETRIEVE_COMP(aniComp, AnimationComponent)
            aniComp->currentFrame = 0;
            aniComp->totalFrames = totalFrames;
            aniComp->width = width;
            aniComp->height = height;
            aniComp->columnCount = columnCount;
            aniComp->dieAfterAnimation = dieAfterAnimation;
            aniComp->movementAnimation = movementAnimation;
        }

        static void addGraphic(Entity* asset, std::string filepath, long zBuffer = 1000, bool drawCentered = true, bool rotate = false) {
            RETRIEVE_COMP(graphicComp, GraphicComponent)
            graphicComp->filepath = filepath;
            graphicComp->zBuffer = zBuffer;
            graphicComp->drawCentered = drawCentered;
            graphicComp->rotate = rotate;
        }

        static void addWorldPlayer(Entity* asset, int player, int team) {
            RETRIEVE_COMP(worldComp, WorldComponent)
            worldComp->playerList.emplace_back(player, team, NO_ENTITY, NO_ENTITY);
        }

        static void addWorldEnemySpawn(Entity* asset, int enemy_spawn_point, AssetID waves, int enemy_goal_point) {
            RETRIEVE_COMP(worldComp, WorldComponent)
            worldComp->enemySpawnList.emplace_back(enemy_spawn_point, waves, enemy_goal_point);
        }

        static void addWorldEnemyGoal(Entity* asset, int enemy_goal_point, int player, int team) {
            RETRIEVE_COMP(worldComp, WorldComponent)
            worldComp->enemyGoalList.emplace_back(enemy_goal_point, player, team, NO_ENTITY);
        }

        static Entity* buildWorld(AsData data, AssetID map) {
            auto asset = createEmptyAsset(data);
            asset->addComponent<WorldComponent>();
            addInit(asset, map);
            return asset;
        }

        static void addMapPlayerSpawn(Entity* asset, float x, float y, AssetID playerRole) {
            RETRIEVE_COMP(mapComp, MapComponent)
            mapComp->playerSpawns.emplace_back(x, y, playerRole, NO_ENTITY);
        }
        static void addMapPlayerSpawn(Entity* asset, VF2 pos, AssetID playerRole) {
            addMapPlayerSpawn(asset, pos.x, pos.y, playerRole); }
        static void addMapPlayerSpawn(Entity* asset, VF2s positions, AssetID playerRole) {
            for (auto& pos : positions) addMapPlayerSpawn(asset, pos, playerRole); }

        static void addMapEnemySpawn(Entity* asset, float x, float y) {
            RETRIEVE_COMP(mapComp, MapComponent)
            mapComp->enemySpawns.emplace_back(x, y);
        }
        static void addMapEnemySpawn(Entity* asset, VF2 pos) {
            addMapEnemySpawn(asset, pos.x, pos.y); }
        static void addMapEnemySpawn(Entity* asset, VF2s positions) {
            for (auto& pos : positions) addMapEnemySpawn(asset, pos); }

        static void addMapEnemyGoal(Entity* asset, float x, float y) {
            RETRIEVE_COMP(mapComp, MapComponent)
            mapComp->enemyGoals.emplace_back(x, y);
        }
        static void addMapEnemyGoal(Entity* asset, VF2 pos) {
            addMapEnemyGoal(asset, pos.x, pos.y); }
        static void addMapEnemyGoal(Entity* asset, VF2s positions) {
            for (auto& pos : positions) addMapEnemyGoal(asset, pos); }

        static void addMapData(Entity* asset, AssetID playerRole, VF2s players, VF2s enemies, VF2s goals) {
            addMapPlayerSpawn(asset, players, playerRole);
            addMapEnemySpawn(asset, enemies);
            addMapEnemyGoal(asset, goals);
        }

        static Entity* buildMap(AsData data, int tileSize, int x, int y, int width, int height) {
            auto asset = createEmptyAsset(data);
            asset->addComponent<MapComponent>(width, height);
            asset->addComponent<TilesComponent>(tileSize, width, height);
            asset->addComponent<PositionComponent>(x, y);
            return asset;
        }

        static Entity* buildPlayerRole(AsData data, AssetIDs playerList, bool blackListed) {
            auto asset = createEmptyAsset(data);
            asset->addComponent<PlayerRoleComponent>(playerList, blackListed);
            return asset;
        }

        static Entity* buildPlayerRole(AsData data, bool blackListed = false) {
            auto asset = createEmptyAsset(data);
            AssetIDs playerList;
            asset->addComponent<PlayerRoleComponent>(playerList, blackListed);
            return asset;
        }

        static Entity* buildPlayer(AsData data, AssetID starterBuilding) {
            auto asset = createEmptyAsset(data);
            asset->addComponent<PlayerComponent>(starterBuilding);
            return asset;
        }

        static Entity* buildMovableUnit(AsData data, long health, float speed, bool groundBased, float dummyRadius) {
            auto asset = createEmptyAsset(data);
            asset->addComponent<PositionComponent>(0.0, 0.0);
            asset->addComponent<MovementComponent>(speed, groundBased);
            asset->addComponent<CollisionComponent>(dummyRadius);
            asset->addComponent<HealthComponent>(health);
            asset->addComponent<SelectableComponent>();
            asset->addComponent<ProjectileTargetComponent>();
            asset->addComponent<ProjectileHittableComponent>();
            return asset;
        }

        static Entity* buildWall(AsData data, bool tilesCheck, bool colCheck, bool occupy, bool impassable,
            float width, float height) {
            //
            auto asset = createEmptyAsset(data);
            asset->addComponent<SupDef::PositionComponent>(0.0, 0.0);
            asset->addComponent<SupDef::ImmovableComponent>(tilesCheck, colCheck, occupy, impassable);
            asset->addComponent<SupDef::CollisionComponent>(3.0);
            asset->addComponent<SelectableComponent>();
            addRectangleShape(asset, width, height);
            return asset;
        }

        static Entity* buildImmovableUnit(AsData data, bool tilesCheck, bool colCheck, bool occupy, bool impassable) {
            auto asset = createEmptyAsset(data);
            asset->addComponent<SupDef::PositionComponent>(0.0, 0.0);
            asset->addComponent<SupDef::ImmovableComponent>(tilesCheck, colCheck, occupy, impassable);
            asset->addComponent<SupDef::CollisionComponent>(3.0);
            asset->addComponent<SupDef::SelectableComponent>();
            asset->addComponent<ProjectileHittableComponent>();
            return asset;
        }

        static Entity* buildImmovableUnit(AsData data, bool tilesCheck, bool colCheck, bool occupy, bool impassable,
                float width, float height) {
            //
            auto asset = buildImmovableUnit(data, tilesCheck, colCheck, occupy, impassable);
            addRectangleShape(asset, width, height);
            return asset;
        }

        static Entity* buildTech(AsData data, TechData techData) {
            auto asset = createEmptyAsset(data);
            asset->addComponent<TechComponent>(techData.techComponent);
            if (techData.techComponent) {
                if (techData.techComponent->applyToWithinInfluence) {
                    asset->addComponent<InfluenceComponent>();
                    asset->addComponent<PositionComponent>(true);
                    addDummyRadius(asset, techData.radius, true);
                }
            }
            return asset;
        }

        static Entity* buildTechForCommand(AsData data, CommandID commandID, TechData techData) {
            auto asset = buildTech(data, techData);
            asset->addComponent<ActiveTechComponent>(commandID);
            return asset;
        }

        static Entity* buildCommand(AsData data) {
            auto asset = createEmptyAsset(data);
            asset->addComponent<CommandComponent>();
            return asset;
        }

        static Entity* buildCommand(AsData data, AssetID techName, TechData techData) {
            auto asset = buildCommand(data);
            data.assetID = techName;
            buildTechForCommand(data, asset->assetID, techData);
            return asset;
        }

        static Entity* buildGiftTech(AsData data, TechData techData, AssetIDs techsToApply) {
            auto asset = buildTech(data, techData);
            asset->addComponent<GiftTechComponent>(techsToApply);
            return asset;
        }

        static Entity* buildGiftTech(AsData data, TechData techData, AssetID techToApply) {
            AssetIDs assets;
            assets.push_back(techToApply);
            return buildGiftTech(data, techData, assets);
        }

        static Entity* buildWeapon(AsData data, AssetID projectile, Damage* damage, Cooldown cooldown, float radius) {
            auto asset = createEmptyAsset(data);
            asset->addComponent<PositionComponent>(true);
            asset->addComponent<InfluenceComponent>();
            asset->addComponent<WeaponComponent>(projectile, damage, cooldown);
            addDummyRadius(asset, radius);
            return asset;
        }

        static Entity* buildProjectile(AsData data, ProjectileData projectileData) {
            auto asset = createEmptyAsset(data);
            asset->addComponent<PositionComponent>();
            auto mov = asset->addComponent<MovementComponent>();
            auto pro = asset->addComponent<ProjectileComponent>();
            addDummyRadius(asset, projectileData.radius);
            mov->original_speed = projectileData.speed;
            mov->speed = projectileData.speed;
            mov->isGroundBased = false;
            mov->movementMode = MovementMode::DirectedMotion;
            pro->applyTechsOnHit = projectileData.applyTechsOnHit;
            pro->multiHitCooldown = projectileData.multiHitCooldown;
            pro->homing = projectileData.homing;
            return asset;
        }

    };

}
