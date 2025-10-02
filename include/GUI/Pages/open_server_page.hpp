#pragma once

#include <GUI/page.hpp>


namespace SupDef {

    class OpenServerPage : public Page {
        public:
            OpenServerPage(PageId pageId_) : Page(pageId_) { }

            void initialize() override {
                // SUBSCRIBE_SIMPLE(globalDispatcher, MenuButtonOpenServer, gotoPageOpenServer);
                // SUBSCRIBE_SIMPLE(globalDispatcher, MenuButtonJoinServer, gotoPageJoinServer);
            }

            void build() override {
                addElement<GuiLabel>(GuiElementStyle::Default, 10, 10, "Server joinen");

                addButton<ClosePageEvent>(GuiElementStyle::Default, 10,  60, 160, 28, "Zurück");
            }

            bool isBlocking() override { return true; }
            bool isCovering() override { return true; }

    };    
    
}
