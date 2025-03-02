#include <Game/asset.hpp>

#pragma once


namespace SupDef {

    class Rules {
        public:
            void to_json(json& j) const {
                // if(assets.empty()) return;
                // for (const auto& [id, asset] : assets) {
                //     j[SG_ASSETS][id][SGA_NAME] = asset->name;
                //     j[SG_ASSETS][id][SGA_DESCRIPTION] = asset->description;
                    
                //     for (const auto& [compName, compData] : asset->components) {
                //         j[SG_ASSETS][id][SGA_COMPONENTS][compName] = compData;
                //     }
                // }
            }

            void from_json(const json& j) {
                // assets.clear();
                // if(j.is_null()) return;
                // if(j[SG_ASSETS].is_null()) return;
                // for (const auto& [id, assetData] : j[SG_ASSETS].items()) {
                //     auto asset = std::make_unique<Asset>(id, assetData[SGA_NAME], assetData[SGA_DESCRIPTION]);
                //     for (const auto& [compName, compData] : assetData[SGA_COMPONENTS].items()) {
                //         asset->components[compName] = compData;
                //     }
                //     assets[id] = std::move(asset);
                // }
            }

            void serialize(const std::string& filename) const {
                json j;
                to_json(j);
                std::ofstream file(filename);
                file << j.dump(4);
            }

            void deserialize(const std::string& filename) {
                std::ifstream file(filename);
                json j;
                file >> j;
                from_json(j);
            }
        
            Asset* getAsset(const AssetID& id) {
                auto it = assets.find(id);
                return (it != assets.end()) ? it->second.get() : nullptr;
            }
        
        private:
            std::unordered_map<AssetID, UAsset> assets;

    };

    using URules = std::unique_ptr<Rules>;

}
