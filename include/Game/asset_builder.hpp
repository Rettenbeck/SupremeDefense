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

        static void addInit(Entity* asset, AssetID assetID) {
            assert(asset);
            auto initComp = asset->retrieveComponent<InitContainerComponent>();
            assert(initComp);
            initComp->contained.push_back(assetID);
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

        static void addDummyRadius(Entity* asset, float radius) {
            assert(asset);
            auto colComp = asset->retrieveComponent<CollisionComponent>();
            assert(colComp);
            colComp->dummyRadius = radius;
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

        static Entity* buildMovableUnit(AsData data, float speed, bool groundBased, float dummyRadius) {
            auto asset = createEmptyAsset(data);
            asset->addComponent<PositionComponent>(0.0, 0.0);
            asset->addComponent<MovementComponent>(speed, groundBased);
            asset->addComponent<CollisionComponent>(dummyRadius);
            asset->addComponent<SelectableComponent>();
            return asset;
        }

        static Entity* buildImmovableUnit(AsData data, bool tilesCheck, bool colCheck, bool occupy, bool impassable) {
            auto asset = createEmptyAsset(data);
            asset->addComponent<SupDef::PositionComponent>(0.0, 0.0);
            asset->addComponent<SupDef::ImmovableComponent>(tilesCheck, colCheck, occupy, impassable);
            asset->addComponent<SupDef::CollisionComponent>(3.0);
            asset->addComponent<SupDef::SelectableComponent>();
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
                    asset->addComponent<PositionComponent>();
                    addDummyRadius(asset, techData.radius);
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

    };

}
