#pragma once

#include <Util/basic.hpp>
#include <Game/constants.hpp>
#include <nlohmann/include.hpp>


namespace SupDef {

    enum class GameStarterStatus {
        NoGame, NewGame, SavedGame, Replay, GameStarted
    };

    struct GameStarterResult {
        GameStarterStatus status;
        std::string message = "";
    };

    class GameStarter {
        private:
            json j_replay, j_assets, j_entities, j_collisions;
            json j_complete;
            GameStarterStatus status = GameStarterStatus::NoGame;
            std::string message = "";

        public:
            GameStarter() { }

            void startNewGame(json& j_assets_, AssetID worldID, PlayerMapExt playerMapExt, int thisPlayer) {
                clear();
                j_assets = j_assets_;
            }

            void startSavedGame(json& j_assets_, AssetID worldID, PlayerMapExt playerMapExt, int thisPlayer) {
                clear();
                j_assets = j_assets_;
            }

            void buildReplay(json& j_replay_, json& j_assets_) {
                clear();
                j_replay = j_replay_;
                j_assets = j_assets_;
            }

            void clear() {
                status = GameStarterStatus::NoGame;
                j_replay = json();
                j_assets = json();
                j_entities = json();
                j_collisions = json();
                j_complete = json();
            }

            GameStarterResult getResult() { return {status, message}; }

    };

    using UGameStarter = std::unique_ptr<GameStarter>;

}
