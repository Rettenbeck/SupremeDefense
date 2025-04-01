#include <App/layers.hpp>
#include <App/constants.hpp>
#include <App/settings.hpp>

#pragma once


namespace SupDef {
    
    class App : public Timer {
        Layers layers;
        UEventDispatcher globalDispatcher;
        USettings settings;

        unsigned init_window_width, init_window_height;
        double frames_per_second, frame_duration;
        long current_frame = -1;
        bool end = false;
        
        public:
            App() {
                Logger::getInstance().setFileOutput(DEFAULT_FILENAME_LOG);
                Logger::getInstance().addMessage(MessageType::Init, MESSAGE_INIT);
                globalDispatcher = std::make_unique<EventDispatcher>();

                settings = std::make_unique<Settings>(DEFAULT_FILENAME_SETTINGS);
                setFramerate(settings->get<double>(S_APP_FRAMERATE, 60.0));

                init_window_width  = settings->get<unsigned>(S_APP_INIT_WINDOW_WIDTH, 1280);
                init_window_height = settings->get<unsigned>(S_APP_INIT_WINDOW_HEIGHT, 720);

                globalDispatcher->subscribe<GameEndEvent>([this](const SupDef::Events& events) {
                    Logger::getInstance().addMessage(MessageType::Info, "Game ended from renderer\n");
                    end = true;
                });
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
            T* getLayer() {
                for(auto& l : layers) {
                    auto cast = dynamic_cast<T*>(l.get());
                    if(cast) return cast;
                }
                return nullptr;
            }

            void doUpdatePreparations() {
                auto game    = getLayer<GameLayer   >();
                auto render  = getLayer<RenderLayer >();
                auto replay  = getLayer<ReplayLayer >();
                auto network = getLayer<NetworkLayer>();
                auto action  = getLayer<ActionRouter>();
                auto gui     = getLayer<GuiLayer    >();

                Game* gamePtr = nullptr;
                GuiManager* guiPtr = nullptr;
                SelectionManager* selPtr = nullptr;
                
                if (game) {
                    gamePtr = game->getGame();
                    selPtr  = game->getSelectionManager();
                }
                if (gui) guiPtr = gui->getGuiManager();

                if(gui) {
                    gui->setGame(gamePtr);
                    gui->setSelectionManager(selPtr);
                }
                if(render) {
                    render->setGame(gamePtr);
                    render->setGuiManager(guiPtr);
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
                    doUpdatePreparations();
                    current_frame++;
                    for (auto& layer : layers) {
                        layer->startMeasurement();
                        layer->update(1.0);
                        layer->stopMeasurement();
                        if(end) { return; }
                    }
                    stopMeasurement();
                    waitUntilElapsed(frame_duration * 1000.0);
                }
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
