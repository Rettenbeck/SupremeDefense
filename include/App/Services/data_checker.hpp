#pragma once

#include <App/Services/file_manager.hpp>
#include <ECS/asset_manager.hpp>
#include <Game/game_state.hpp>
#include <Game/replay.hpp>


namespace SupDef {

    class DataChecker {
        public:
            UAssetManager assetManager;
            UGameState gameState;
            UReplay replay;

            DataChecker() { }

            void clear() {
                assetManager.reset();
                gameState.reset();
                replay.reset();
            }

            bool checkFromFile(FileManager* fileManager) {
                assert(fileManager);
                bool success = true;
                
                clear();
                if (fileManager->has_asset     ) if (!checkAssetManager    (fileManager->j_assets    )) success = false;
                if (fileManager->has_game_state) if (!checkGameState       (fileManager->j_game_state)) success = false;
                if (fileManager->has_replay    ) if (!checkReplay          (fileManager->j_replay    )) success = false;
                return success;
            }

            template <typename T>
            bool check(std::unique_ptr<T>& obj, json& j) {
                try {
                    obj = std::make_unique<T>();
                    obj->from_json(j);
                    return true;
                } catch(const std::exception& e) {
                    obj.reset();
                    std::stringstream ss;
                    ss << "Error on reading " << typeid(T).name() << ": " << e.what();
                    LOG_ERROR(ss.str())
                    return false;
                }
            }

            bool checkAssetManager    (json& j) { return check<AssetManager    >(assetManager    , j); }
            bool checkGameState       (json& j) { return check<GameState       >(gameState       , j); }
            bool checkReplay          (json& j) { return check<Replay          >(replay          , j); }

    };

    using UDataChecker = std::unique_ptr<DataChecker>;

}
