#pragma once

#include <GUI/gui_manager.hpp>


namespace SupDef {

    enum class GM_Network_Page {
        Initial, Server, Client, Joined
    };

    class GuiManagerNetwork : public GuiManager {
        private:
            GM_Network_Page page = GM_Network_Page::Initial;
        
        public:
            ~GuiManagerNetwork() {
                SUBSCRIBE_BEGIN(globalDispatcher, StartGameAsServerStatusEvent)
                    if (!typedEvent.success) doBackToInitial();
                SUBSCRIBE_END
                SUBSCRIBE_BEGIN(globalDispatcher, StartGameAsClientStatusEvent)
                    if (!typedEvent.success) doBackToInitial();
                SUBSCRIBE_END
            }
        
            void update(float deltaTime) {
                GuiManager::update(deltaTime);
                buildPage();
            }

            void buildPage() {
                switch (page) {
                    case GM_Network_Page::Initial:
                        buildPageInitial();
                        break;
                    case GM_Network_Page::Server:
                        buildPageServer();
                        break;
                    case GM_Network_Page::Client:
                        buildPageClient();
                        break;
                    case GM_Network_Page::Joined:
                        buildPageJoined();
                        break;
                    default:
                        break;
                }
            }

            void buildPageInitial() {
                addInput(std::make_unique<GuiInput>(GuiElementStyle::Default, 10, 10, "Name"), GUI_NW_INPUT_KEY_PLAYER_NAME);
                addInput(std::make_unique<GuiInput>(GuiElementStyle::Default, 10, 28, "Port"), GUI_NW_INPUT_KEY_PORT);
                addInput(std::make_unique<GuiInput>(GuiElementStyle::Default, 10, 58, "IP"  ), GUI_NW_INPUT_KEY_IP);
                setInput(GUI_NW_INPUT_KEY_PORT, "9000");
                setInput(GUI_NW_INPUT_KEY_IP, "127.0.0.1");
                // addClickable(std::make_unique<GuiButton>(GuiElementStyle::Default,  10, 60, 100, 28, "Server aufmachen"), doPageInitialStartAsServer);
                // addClickable(std::make_unique<GuiButton>(GuiElementStyle::Default, 120, 60, 100, 28, "Server joinen"), doPageInitialStartAsClient);
            }

            void buildPageServer() {
                // addClickable(std::make_unique<GuiButton>(GuiElementStyle::Default, 10, 160, 100, 28, "Abbrechen"), doBackToInitial);
            }

            void buildPageClient() {
                //
            }

            void buildPageJoined() {
                //
            }

            void handleClickOnGui(GuiElement* element, MouseClick button, json data) {
                if (button == MLEFT) if (handleButton(element)) return;
            }

            void doPageInitialStartAsServer() {
                page = GM_Network_Page::Server;
                auto playerName = getInput(GUI_NW_INPUT_KEY_PLAYER_NAME);
                globalDispatcher->dispatch<SetPlayerNameEvent>(playerName);
                globalDispatcher->dispatch<StartNetworkGameAsServerEvent>();
            }
        
            void doPageInitialStartAsClient() {
                page = GM_Network_Page::Client;
                auto playerName = getInput(GUI_NW_INPUT_KEY_PLAYER_NAME);
                globalDispatcher->dispatch<SetPlayerNameEvent>(playerName);
                globalDispatcher->dispatch<StartNetworkGameAsClientEvent>();
            }

            void doBackToInitial() {
                page = GM_Network_Page::Initial;
                globalDispatcher->dispatch<StopNetworkGameEvent>();
            }
        
    };

    using UGuiManagerNetwork = std::unique_ptr<GuiManagerNetwork>;

}
