#pragma once

#include <App/layer.hpp>
#include <Game/game.hpp>
#include <Action/action_queue.hpp>


namespace SupDef {

    class GameLayer : public Layer {
        private:
            UGame game;
            USelectionManager selectionManager;
            UActionQueue actionQueue;
            bool blockedByNetwork = false;
            long frameCount = 0;
    
        public:
            GameLayer() {
                priority = PRIORITY_GAME;
            }

            void onAttach() override {
                actionQueue = std::make_unique<ActionQueue>();
                selectionManager = std::make_unique<SelectionManager>();
                selectionManager->setGlobalDispatcher(globalDispatcher);
                selectionManager->initialize();

                SUBSCRIBE(GameBlockedByNetworkEvent)
                SUBSCRIBE(ActionsReceivedForGameEvent)
            }

            Game* createGame() {
                game = std::make_unique<Game>();
                game->setGlobalDispatcher(globalDispatcher);
                game->setActionQueue(actionQueue.get());
                game->initialize();
                return game.get();
            }

            void startGameFromStarter() {
                assert(globalDispatcher);
                assert(services);
                auto& starter = services->gameStarter;
                assert(starter);

                if (starter->getStatus() == GameStarterStatus::NewGame) {
                    startNewGame(starter.get());
                    starter->setStatus(GameStarterStatus::GameStarted);
                } else if (starter->getStatus() == GameStarterStatus::SavedGame) {
                    startSavedGame(starter.get());
                    starter->setStatus(GameStarterStatus::GameStarted);
                } else if (starter->getStatus() == GameStarterStatus::Replay) {
                    startReplay(starter.get());
                    starter->setStatus(GameStarterStatus::GameStarted);
                }
            }

            void startNewGame(GameStarter* starter) {
                assert(starter);
                createGame();
                assert(game);
                game->setAssetManager(std::move(starter->assetManager));
                game->startWorld(std::move(starter->initial));
                globalDispatcher->dispatch<PushGamePageEvent>();
            }

            void startSavedGame(GameStarter* starter) {
                startNewGame(starter);
                game->setCollisionTracker(std::move(starter->collisionTracker));
                game->setEntityManager(std::move(starter->entityManager));
                game->setSaveReplay(std::move(starter->replay));
            }
        
            void startReplay(GameStarter* starter) {
                assert(starter);
                createGame();
                assert(game);
                auto initial = std::make_unique<InitialConditions>();
                json j; starter->replay->initial->to_json(j);
                initial->from_json(j);
                assert(!initial->playerMapExt.empty());
                auto& [id, type, ent_id, name] = initial->playerMapExt[0];
                initial->thisPlayer = id;
                game->setAssetManager(std::move(starter->assetManager));
                game->startWorld(std::move(initial));
                game->setLoadReplay(std::move(starter->replay));
                globalDispatcher->dispatch<PushGamePageEvent>();
            }
        
            void update(float deltaTime) override {
                if (blockedByNetwork) return;
                startGameFromStarter();
                if (!game) return;
                game->update(deltaTime);
                frameCount++;
                game->setFrameCount(frameCount);
                assert(game->getThisPlayer());
                dispatch<GameHasUpdatedEvent>(game->getThisPlayer()->id, frameCount);
            }

            DEFINE_EVENT_CALLBACK(GameBlockedByNetworkEvent) {
                blockedByNetwork = event.blocked;
            }
            
            DEFINE_EVENT_CALLBACK(ActionsReceivedForGameEvent) {
                if (!game) return;
                if (game->isReplay()) return;
                assert(actionQueue);
                assert(event.actionQueue);
                actionQueue->clear();
                actionQueue->getActions() = event.actionQueue->getActions();
            }
            
            Game* getGame() { return game.get(); }
            SelectionManager* getSelectionManager() { return selectionManager.get(); }
            ActionQueue* getActionQueue() { return actionQueue.get(); }

    };
    
}
