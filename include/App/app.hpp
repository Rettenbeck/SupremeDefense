#pragma once

#include <App/layers.hpp>


namespace SupDef {
    
    class App : public Timer , public Listener {
        Layers layers;
        UServices services;
        std::vector<std::function<void()>> actions;

        unsigned init_window_width, init_window_height;
        double frames_per_second, frame_duration;
        long current_frame = -1;
        bool changed = false, end = false;
        
        public:
            App() {
                construct_components();
                Logger::getInstance().setFileOutput(DEFAULT_FILENAME_LOG);
                services = std::make_unique<Services>();
                setGlobalDispatcher(services->globalDispatcher.get());
                setFramerate(services->settings->get<double>(S_APP_FRAMERATE, 60.0));

                init_window_width  = services->settings->get<unsigned>(S_APP_INIT_WINDOW_WIDTH, 1280);
                init_window_height = services->settings->get<unsigned>(S_APP_INIT_WINDOW_HEIGHT, 720);
                
                globalDispatcher->SUBSCRIBE(GameEndEvent)
                globalDispatcher->SUBSCRIBE(UpdateAppEvent)
                globalDispatcher->SUBSCRIBE(StartTestNewGameEvent)
                globalDispatcher->SUBSCRIBE(StartTestSavedGameEvent)
                globalDispatcher->SUBSCRIBE(StartTestReplayEvent)

                fillLayers();
            }

            ~App() { layers.clear(); }

            void addLayer(ULayer layer) {
                layer->setGlobalDispatcher(globalDispatcher);
                layer->setServices(services.get());
                layer->onAttach();
                layers.push_back(std::move(layer));
                std::sort(layers.begin(), layers.end(), [](const auto& a, const auto& b) {
                    return a->getPriority() < b->getPriority();
                });
            }
        
            template <typename T>
            void removeLayer() {
                for (auto it = layers.begin(); it != layers.end(); it++) {
                    auto ptr = (*it).get();
                    if (dynamic_cast<T*>(ptr)) {
                        ptr->onDetach();
                        layers.erase(it);
                    }
                }
            }

            template <typename T>
            T* getLayer() {
                for(auto& l : layers) {
                    auto cast = dynamic_cast<T*>(l.get());
                    if(cast) return cast;
                }
                return nullptr;
            }

            template <typename T>
            T* retrieveLayer() {
                auto ptr = getLayer<T>();
                if (ptr) return ptr;
                addLayer(std::make_unique<T>());
                ptr = getLayer<T>();
                assert(ptr);
                return ptr;
            }

            void doUpdatePreparations() {
                auto game    = getLayer<GameLayer   >();
                auto render  = getLayer<RenderLayer >();
                auto network = getLayer<NetworkLayer>();
                auto gui     = getLayer<GuiLayer    >();

                Game* gamePtr = nullptr;
                SocketBackend* socketBackendPtr = nullptr;
                GuiManager* guiPtr = nullptr;
                SelectionManager* selPtr = nullptr;
                
                if (game) {
                    gamePtr = game->getGame();
                    selPtr  = game->getSelectionManager();
                }
                if (network) socketBackendPtr = network->getSocketBackend();
                if (gui) guiPtr = gui->getGuiManager();

                if(gui) {
                    gui->setGame(gamePtr);
                    gui->setSelectionManager(selPtr);
                    gui->setSocketBackend(socketBackendPtr);
                }
                if(render) {
                    render->setGame(gamePtr);
                    render->setGuiManager(guiPtr);

                    float fc_total = 1000.0 / getLastMeasurement();
                    float fc_renderer = 1000.0 / render->getLastMeasurement();
                    float fc_game = 0.0;
                    if (game) fc_game = 1000.0 / game->getLastMeasurement();
                    render->setFramecountData(fc_total, fc_renderer, fc_game);
                }
            }

            void start() {
                auto render = getLayer<RenderLayer>();
                render->setWindowSize(init_window_width, init_window_height);
                for (auto& layer : layers) {
                    layer->onStart();
                }
            }

            void run() {
                start();
                current_frame = 0;
                while (true) {
                    startMeasurement();
                    processQueuedActions();
                    doUpdatePreparations();
                    current_frame++;
                    for (auto& layer : layers) {
                        layer->startMeasurement();
                        layer->update(1.0);
                        layer->stopMeasurement();
                        if (changed) doUpdatePreparations();
                        if(end) { return; }
                        changed = false;
                    }
                    stopMeasurement();
                    waitUntilElapsed(frame_duration * 1000.0);
                }
            }

            void processQueuedActions() {
                for (const auto& action : actions) {
                    action();
                }
                actions.clear();
            }

            DEFINE_EVENT_CALLBACK(StartTestNewGameEvent) {
                assert(services->assetService);
                assert(services->gameStarter);

                PlayerMapExt playerMapExt;
                playerMapExt.emplace_back(1, AS_PLAYER_HUMAN, NO_ENTITY, "Player 1");
                playerMapExt.emplace_back(2, AS_PLAYER_HUMAN, NO_ENTITY, "Player 2");
                playerMapExt.emplace_back(3, AS_PLAYER_HUMAN, NO_ENTITY, "Player 3");

                auto initial = std::make_unique<InitialConditions>();
                initial->worldID = AS_WORLD_WINTER_MAUL;
                initial->playerMapExt = playerMapExt;
                initial->thisPlayer = 1;

                auto s = services->gameStarter->startNewGame(services->assetService.get(), std::move(initial));
                std::cout << "New Game to be started. Success? " << s << "\n";
            }
            
            DEFINE_EVENT_CALLBACK(StartTestSavedGameEvent) {
                assert(services->fileManager);
                assert(services->dataChecker);
                assert(services->gameStarter);
                
                if (!services->fileManager->loadFromFile("state.txt")) return;
                services->dataChecker->checkFromFile(services->fileManager.get());
                auto s = services->gameStarter->startSavedGame(services->dataChecker.get(), 1);
                std::cout << "Saved Game to be started. Success? " << s << "\n";
            }

            DEFINE_EVENT_CALLBACK(StartTestReplayEvent) {
                assert(services->fileManager);
                assert(services->dataChecker);
                assert(services->gameStarter);
                if (!services->fileManager->loadFromFile("state.txt")) return;
                services->dataChecker->checkFromFile(services->fileManager.get());
                auto s = services->gameStarter->startReplay(services->dataChecker.get());
                std::cout << "Replay to be started. Success? " << s << "\n";
            }

            DEFINE_EVENT_CALLBACK(GameEndEvent) {
                end = true;
            }
            
            DEFINE_EVENT_CALLBACK(UpdateAppEvent) {
                changed = true;
            }
            
            void setFramerate(double fps) {
                frames_per_second = fps;
                frame_duration = 1.0 / fps;
            }

            double getFramerate() { return frames_per_second; }
            double getFrameDuration() { return frame_duration; }
            double getTrueFramerate() { return getLastMeasurement(); }
            double getTrueFrameDuration() { return 1.0 / getLastMeasurement(); }

            Layers& getLayers() {
                return layers;
            }

            void fillLayers() {
                addLayer(std::make_unique<SupDef::GuiLayer>());
                addLayer(std::make_unique<SupDef::GameLayer>());
                addLayer(std::make_unique<SupDef::RenderLayer>());
                addLayer(std::make_unique<SupDef::NetworkLayer>());
            }

    };
    
    using UApp = std::unique_ptr<App>;

}
