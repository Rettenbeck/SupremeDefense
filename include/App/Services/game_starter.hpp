#pragma once

#include <Game/constants.hpp>
#include <App/Services/asset_service.hpp>
#include <App/Services/data_checker.hpp>
#include <Game/replay.hpp>


namespace SupDef {

    enum class GameStarterStatus {
        NoGame, NewGame, PrepareNewGame, SavedGame, Replay, GameStarted
    };

    struct GameStarterResult {
        GameStarterStatus status;
        std::string message = "";
    };

    class GameStarter {
        private:
            GameStarterStatus status = GameStarterStatus::NoGame;
            std::string message = "";

        public:
            UInitialConditions initial;
            UAssetManager assetManager;
            UGameState gameState;
            // UEntityManager entityManager;
            // UCollisionTracker collisionTracker;
            UReplay replay;

            GameStarter() { }


            // ### New game ############################################## //
            bool startNewGame(UInitialConditions initial_, UAssetManager assetManager_) {
                clear();
                if (!initial_) return false;
                if (!assetManager_) return false;
                initial = std::move(initial_);
                assetManager = std::move(assetManager_);
                status = GameStarterStatus::NewGame;
                return true;
            }

            bool startNewGame(AssetService* assetService, UInitialConditions initial_) {
                assert(assetService);
                return startNewGame(std::move(initial_), std::move(assetService->getAssetManagerClone()));
            }

            bool startNewGame(UAssetManager assetManager_, AssetID worldID, PlayerMapExt playerMapExt, int thisPlayer) {
                auto initial_ = std::make_unique<InitialConditions>();
                initial_->worldID = worldID;
                initial_->playerMapExt = playerMapExt;
                initial_->thisPlayer = thisPlayer;
                return startNewGame(std::move(initial_), std::move(assetManager_));
            }

            
            // ### Saved game ############################################## //
            bool startSavedGame(UAssetManager assetManager_, UGameState gameState_, UReplay replay_, int thisPlayer) {
                clear();
                if (!assetManager_) return false;
                if (!gameState_) return false;
                if (!gameState_->entityManager) return false;
                if (!gameState_->collisionTracker) return false;
                // if (!entityManager_) return false;
                // if (!collisionTracker_) return false;
                if (!replay_) return false;
                if (!replay_->initial) return false;
                initial = std::move(replay_->initial);
                assetManager = std::move(assetManager_);
                gameState = std::move(gameState_);
                // entityManager = std::move(entityManager_);
                // collisionTracker = std::move(collisionTracker_);
                replay = std::move(replay_);
                initial->thisPlayer = thisPlayer;
                status = GameStarterStatus::SavedGame;
                return true;
            }

            bool startSavedGame(DataChecker* dataChecker, int thisPlayer) {
                assert(dataChecker);
                if (!dataChecker->assetManager) return false;
                if (!dataChecker->gameState) return false;
                if (!dataChecker->gameState->entityManager) return false;
                if (!dataChecker->gameState->collisionTracker) return false;
                // if (!dataChecker->entityManager) return false;
                // if (!dataChecker->collisionTracker) return false;
                if (!dataChecker->replay) return false;
                return startSavedGame(std::move(dataChecker->assetManager), std::move(dataChecker->gameState),
                                        std::move(dataChecker->replay), thisPlayer);
            }

            
            // ### Replay ############################################## //
            bool startReplay(UAssetManager assetManager_, UReplay replay_) {
                clear();
                if (!assetManager_) return false;
                if (!replay_) return false;
                if (!replay_->initial) return false;
                assetManager = std::move(assetManager_);
                replay = std::move(replay_);
                status = GameStarterStatus::Replay;
                return true;
            }

            bool startReplay(DataChecker* dataChecker) {
                assert(dataChecker);
                if (!dataChecker->assetManager) return false;
                if (!dataChecker->replay) return false;
                return startReplay(std::move(dataChecker->assetManager), std::move(dataChecker->replay));
            }

            void clear() {
                status = GameStarterStatus::NoGame;
                initial.reset();
                assetManager.reset();
                gameState.reset();
                replay.reset();
            }

            void setStatus(GameStarterStatus status_) { status = status_; }
            GameStarterStatus getStatus() { return status; }

    };

    using UGameStarter = std::unique_ptr<GameStarter>;

}
