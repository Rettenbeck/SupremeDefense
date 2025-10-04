#pragma once

#include <GUI/page.hpp>


namespace SupDef {

    class StartPage : public Page {
        public:
            StartPage(PageTypeId pageTypeId_) : Page(pageTypeId_) { }

            void initialize() override {
                SUBSCRIBE(MenuButtonOpenServerEvent)
                SUBSCRIBE(MenuButtonJoinServerEvent)
                SUBSCRIBE(MenuButtonOpenModListEvent)
            }

            void build() override {
                addElement<GuiLabel>(10, 10, "Supreme Defense");

                addButton<MenuButtonOpenServerEvent>(10,  60, 160, 28, "Server aufmachen");
                addButton<MenuButtonJoinServerEvent>(10, 100, 160, 28, "Server joinen");
                addButton<StartTestGameEvent>(10, 140, 160, 28, "Testspiel starten");
                addButton<MenuButtonOpenModListEvent>(10, 180, 160, 28, "Mods");

                addButton<GameEndEvent>(10, 260, 160, 28, "Beenden");
            }

            DEFINE_EVENT_CALLBACK(MenuButtonOpenServerEvent) {
                pushPage(PAGE_TYPE_ID_OPEN_SERVER);
            }

            
            DEFINE_EVENT_CALLBACK(MenuButtonJoinServerEvent) {
                pushPage(PAGE_TYPE_ID_JOIN_SERVER);
            }

            DEFINE_EVENT_CALLBACK(MenuButtonOpenModListEvent) {
                pushPage(PAGE_TYPE_ID_MOD_LIST);
            }

            bool isBlocking() override { return true; }
            bool isCovering() override { return true; }

    };    
    
}
