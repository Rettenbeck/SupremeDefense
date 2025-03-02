#include <ECS/entity.hpp>
#include <Game/constants.hpp>

#pragma once


namespace SupDef {

    // struct Asset {
    //     AssetID id;
    //     std::string name;
    //     std::string description;
    //     std::unordered_map<AssetID, json> components;
        
    //     Asset(AssetID id, std::string name, std::string description)
    //         : id(std::move(id)), name(std::move(name)), description(std::move(description)) {}
    // };

    class Asset {
        public:
            AssetID assetID;
            std::string name;
            std::string description;
            
            Asset(AssetID assetID, std::string name, std::string description)
                : assetID(std::move(assetID)), name(std::move(name)), description(std::move(description)),
                Entity(NO_ENTITY) {}
    };

    using UAsset = std::unique_ptr<Asset>;
    using Assets = std::vector<UAsset>;

}
