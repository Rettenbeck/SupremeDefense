#pragma once

#include <App/layers.hpp>
#include <App/constants.hpp>
#include <App/settings.hpp>


namespace SupDef {
    
    class App : public Timer {
        Layers layers;
        UEventDispatcher globalDispatcher;
        USettings settings;
        std::vector<std::function<void()>> actions;

        unsigned init_window_width, init_window_height;
        double frames_per_second, frame_duration;
        long current_frame = -1;
        bool end = false;
        bool changed = false;
        
        public:
            App() {
                Logger::getInstance().setFileOutput(DEFAULT_FILENAME_LOG);
                LOG(Init, MESSAGE_INIT)
                globalDispatcher = std::make_unique<EventDispatcher>();

                settings = std::make_unique<Settings>(DEFAULT_FILENAME_SETTINGS);
                setFramerate(settings->get<double>(S_APP_FRAMERATE, 60.0));

                init_window_width  = settings->get<unsigned>(S_APP_INIT_WINDOW_WIDTH, 1280);
                init_window_height = settings->get<unsigned>(S_APP_INIT_WINDOW_HEIGHT, 720);
                
                SUBSCRIBE_BEGIN(globalDispatcher, GameEndEvent)
                    LOG(Info, "Game ended from renderer")
                    end = true;
                SUBSCRIBE_END
                SUBSCRIBE_BEGIN(globalDispatcher, UpdateAppEvent)
                    changed = true;
                SUBSCRIBE_END
                SUBSCRIBE_ACTION_SIMPLE(globalDispatcher, StartNetworkGameAsServerEvent, actions, startNetworkGameAsServer(typedEvent.port))
                SUBSCRIBE_ACTION_SIMPLE(globalDispatcher, StartNetworkGameAsClientEvent, actions, startNetworkGameAsClient(typedEvent.ip, typedEvent.port))
                SUBSCRIBE_ACTION_SIMPLE(globalDispatcher, CompleteServerEvent, actions, completeServer())
                SUBSCRIBE_ACTION_SIMPLE(globalDispatcher, StopNetworkGameEvent, actions, stopNetworkGame())
                SUBSCRIBE_ACTION_SIMPLE(globalDispatcher, StartTestGameEvent, actions, startGame())
            }

            ~App() { layers.clear(); }

            void addLayer(ULayer layer) {
                layer->setGlobalDispatcher(globalDispatcher.get());
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
                auto replay  = getLayer<ReplayLayer >();
                auto network = getLayer<NetworkLayer>();
                auto action  = getLayer<ActionRouter>();
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
                if(action) action->prepare(game, replay, network);
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

            bool startNetworkGameAsServer(unsigned short port) {
                // assert(globalDispatcher);
                // auto success = retrieveLayer<NetworkLayer>()->startNetworkGameAsServer(port);
                // globalDispatcher->dispatch<StartGameAsServerStatusEvent>(success);
                // return success;
                return true;
            }

            bool startNetworkGameAsClient(const std::string& ip, unsigned short port) {
                // assert(globalDispatcher);
                // auto success = retrieveLayer<NetworkLayer>()->startNetworkGameAsClient(ip, port);
                // globalDispatcher->dispatch<StartGameAsClientStatusEvent>(success);
                // return success;
                return true;
            }

            void stopNetworkGame() {
                removeLayer<NetworkLayer>();
            }

            void completeServer() {
                // retrieveLayer<NetworkLayer>()->completeServer();
            }

            void startGame(UAssetManager assetManager, AssetID worldID, PlayerMapExt playerMapExt, int thisPlayer) {
                assert(assetManager);
                if (worldID.empty()) {
                    LOG_ERROR("No worldID given")
                    return;
                }
                removeLayer<GameLayer>();
                auto gameLayer = retrieveLayer<GameLayer>();
                auto game = gameLayer->getGame();
                assert(game);
                game->setAssetManager(std::move(assetManager));
                game->startWorld(worldID, playerMapExt, thisPlayer);
                assert(globalDispatcher);
                globalDispatcher->dispatch<GotoPageEvent>(PAGE_ID_GAME);
            }

            void startGame() {
                auto am = std::make_unique<AssetManager>();
                BuildAssets::build(am.get());
                AssetID worldID = AS_WORLD_WINTER_MAUL;
                PlayerMapExt playerMapExt;
                playerMapExt.emplace_back(1, AS_PLAYER_HUMAN, NO_ENTITY);
                playerMapExt.emplace_back(2, AS_PLAYER_HUMAN, NO_ENTITY);
                playerMapExt.emplace_back(3, AS_PLAYER_HUMAN, NO_ENTITY);
                startGame(std::move(am), worldID, playerMapExt, 1);
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

            EventDispatcher* getGlobalDispatcher() { return globalDispatcher.get(); }
            
    };
    
    using UApp = std::unique_ptr<App>;

}
