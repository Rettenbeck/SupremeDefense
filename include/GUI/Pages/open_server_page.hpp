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
                assert(guiManager);
                guiManager->add(std::make_unique<GuiLabel>(GuiElementStyle::Default, 10, 10, "Server aufmachen"));

                guiManager->addClickable<ClosePageEvent>(std::make_unique<GuiButton>(GuiElementStyle::Default, 10,  60, 160, 28, "Zurück"));
            }

            bool isBlocking() override { return true; }
            bool isCovering() override { return true; }

    };    
    
}
