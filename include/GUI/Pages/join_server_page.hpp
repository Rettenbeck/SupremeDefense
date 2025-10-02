#pragma once

#include <GUI/page.hpp>


namespace SupDef {

    class JoinServerPage : public Page {
        private:
            TableData data;

        public:
            JoinServerPage(PageId pageId_) : Page(pageId_) { }

            void initialize() override {
                SUBSCRIBE_SIMPLE(globalDispatcher, RequestServerListRefreshAnswerEvent,
                    onRequestServerListRefreshAnswerEvent(typedEvent.ok, typedEvent.message));
            }

            void build() override {
                addElement<GuiLabel>(GuiElementStyle::Default, 10, 10, "Server joinen");
                addButton<ClosePageEvent>(GuiElementStyle::Default, 10,  60, 160, 28, "Zurück");
                // addButton<RequestServerListRefreshEvent>(GuiElementStyle::Default, 10, 160, 160, 28, "Refresh");

                auto func = [&]() {
                    std::cout << "Button Refresh pushed\n";
                    globalDispatcher->dispatch<RequestServerListRefreshEvent>();
                };

                addButtonCB(func, GuiElementStyle::Default, 20, 160, 160, 28, "Refresh");

                auto ptr_table = addElement<GuiTable>(GuiElementStyle::Default, 200, 60, 360, 160);
                assert(ptr_table);
                TableLine head = {"Servername", "IP", "Beschreibung"};
                ptr_table->setHead(head);
                ptr_table->clear();
                ptr_table->appendData(data);
            }

            void onRequestServerListRefreshAnswerEvent(bool ok, std::string message) {
                std::cout << "RequestServerListRefreshAnswerEvent: Ok? " << ok << "; message: " << message << "\n";
            }

            bool isBlocking() override { return true; }
            bool isCovering() override { return true; }

    };    
    
}
