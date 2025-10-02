#pragma once

#include <GUI/page.hpp>


namespace SupDef {

    class OpenServerPage : public Page {
        public:
            OpenServerPage(PageId pageId_) : Page(pageId_) { }

            ~OpenServerPage() {
                dispatch<RequestOpenServerCloseEvent>();
            }

            void initialize() override {
                dispatch<RequestServerOpenEvent>();
            }

            void build() override {
                addElement<GuiLabel>(GuiElementStyle::Default, 10, 10, "Server joinen");

                addButton<ClosePageEvent>(GuiElementStyle::Default, 10,  60, 160, 28, "Zurück");
            }

            bool isBlocking() override { return true; }
            bool isCovering() override { return true; }

    };    
    
}
