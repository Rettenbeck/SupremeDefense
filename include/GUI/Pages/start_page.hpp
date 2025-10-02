#pragma once

#include <GUI/page.hpp>


namespace SupDef {

    class StartPage : public Page {
        public:
            StartPage(PageId pageId_) : Page(pageId_) { }

            void initialize() override {
                SUBSCRIBE(MenuButtonOpenServerEvent)
                SUBSCRIBE(MenuButtonJoinServerEvent)
            }

            void build() override {
                addElement<GuiLabel>(GuiElementStyle::Default, 10, 10, "Supreme Defense");

                addButton<MenuButtonOpenServerEvent>(GuiElementStyle::Default, 10,  60, 160, 28, "Server aufmachen");
                addButton<MenuButtonJoinServerEvent>(GuiElementStyle::Default, 10, 100, 160, 28, "Server joinen");
                addButton<StartTestGameEvent>(GuiElementStyle::Default, 10, 140, 160, 28, "Testspiel starten");
                addButton<GameEndEvent>(GuiElementStyle::Default, 10, 220, 160, 28, "Beenden");
            }

            DEFINE_EVENT_CALLBACK(MenuButtonOpenServerEvent) {
                pushPage(PAGE_ID_OPEN_SERVER);
            }

            
            DEFINE_EVENT_CALLBACK(MenuButtonJoinServerEvent) {
                pushPage(PAGE_ID_JOIN_SERVER);
            }

            bool isBlocking() override { return true; }
            bool isCovering() override { return true; }

    };    
    
}
