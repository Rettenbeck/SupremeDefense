#include <Game/game.hpp>
#include <Game/BuildAssets/constants.hpp>

#pragma once


namespace SupDef {

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
            auto asset = data.am->createAsset(data.assetID);
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
            assert(asset);
            auto resComp = asset->retrieveComponent<ResourceComponent>();
            assert(resComp);
            resComp->addResource(id, amount, gatherRate, maxCapacity);
        }

        static void addRequiredResource(Entity* asset, ResourceID id, long amount) {
            //
            assert(asset);
            auto req = asset->retrieveComponent<RequirementComponent>();
            req->add(std::make_unique<SupDef::Resource>(id, amount));
        }

        static void addInit(Entity* asset, AssetID assetID) {
            assert(asset);
            auto initComp = asset->retrieveComponent<InitContainerComponent>();
            assert(initComp);
            initComp->contained.push_back(assetID);
        }

        static void addInit(Entity* asset, AssetIDs assetIDs) {
            for (auto& assetID : assetIDs) {
                addInit(asset, assetID);
            }
        }

        static void addPlayerSpawn(AsData data, Entity* asset, float x, float y, AssetIDs players) {
            assert(asset);
            AsData dataSpawn = data;
            int i = 1;
            do {
                std::stringstream ss; ss << data.assetID << AFFIX_SPAWN << i;
                dataSpawn.assetID = ss.str();
                if (!data.am->getAsset(dataSpawn.assetID)) break;
            } while (i++ < 1000);
            auto assetSpawn = createEmptyAsset(dataSpawn);
            assetSpawn->addComponent<PlayerSpawnComponent>(players);
            assetSpawn->addComponent<InitPositionComponent>(x, y);
            addInit(asset, dataSpawn.assetID);
        }

        static void addPlayerSpawn(AsData data, Entity* asset, float x, float y, AssetID player) {
            AssetIDs players;
            players.push_back(player);
            addPlayerSpawn(data, asset, x, y, players);
        }

        static void addPlayerSpawns(AsData data, Entity* asset, VF2s positions, AssetIDs players) {
            for (auto& position : positions) {
                addPlayerSpawn(data, asset, position.x, position.y, players);
            }
        }

        static void addPlayerSpawns(AsData data, Entity* asset, VF2s positions, AssetID player) {
            AssetIDs players;
            players.push_back(player);
            addPlayerSpawns(data, asset, positions, players);
        }

        static void addRectangleShape(Entity* asset, float width, float height, bool calcDummyRadius = true) {
            assert(asset);
            auto colComp = asset->retrieveComponent<CollisionComponent>();
            assert(colComp);
            if (calcDummyRadius) {
                colComp->dummyRadius = std::min(width, height);
            }
            colComp->addShape(std::make_unique<SupDef::RectangleShape>(width, height));
        }

        static void addDummyRadius(Entity* asset, float radius, bool isInfluence = false) {
            assert(asset);
            auto colComp = asset->retrieveComponent<CollisionComponent>();
            assert(colComp);
            colComp->dummyRadius = radius;
            colComp->isInfluence = isInfluence;
        }

        static Entity* buildWorld(AsData data, AssetID map) {
            auto asset = createEmptyAsset(data);
            asset->addComponent<WorldComponent>();
            addInit(asset, map);
            return asset;
        }

        static Entity* buildMap(AsData data, int tileSize, int x, int y, int width, int height) {
            auto asset = createEmptyAsset(data);
            asset->addComponent<MapComponent>(width, height);
            asset->addComponent<TilesComponent>(tileSize, width, height);
            asset->addComponent<PositionComponent>(x, y);
            return asset;
        }

        static Entity* buildPlayer(AsData data) {
            auto asset = createEmptyAsset(data);
            asset->addComponent<PlayerComponent>();
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
