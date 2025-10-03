#pragma once

#include <GUI/page.hpp>


namespace SupDef {

    class JoinServerPage : public Page {
        private:
            std::vector<std::string> data;

        public:
            JoinServerPage(PageTypeId pageTypeId_) : Page(pageTypeId_) { }

            void initialize() override {
                SUBSCRIBE(RequestServerListRefreshAnswerEvent)
                SUBSCRIBE(RetrievedServerListEvent)
                dispatch<RequestServerListRefreshEvent>();
            }

            void build() override {
                addElement<GuiLabel>(GuiElementStyle::Default, 10, 10, "Server joinen");

                addButton<RequestServerListRefreshEvent>(GuiElementStyle::Default, 200, 60, 160, 28, "Refresh");

                auto ptr_table = addElement<GuiTable>(GuiElementStyle::Default, 200, 92, 760, 460, "Server list");
                assert(ptr_table);
                TableLine head = {"Servername", "IP", "Beschreibung"};
                ptr_table->setHead(head);
                ptr_table->clear();
                ptr_table->appendData(data);
                
                addClickableEvent<GuiButton, ClosePageEvent>(
                    std::make_tuple(GuiElementStyle::Default, 10,  60, 160, 28, "Zurück"),
                    std::make_tuple(pageId)
                );
            }

            DEFINE_EVENT_CALLBACK(RequestServerListRefreshAnswerEvent) {
                if (!event.ok) std::cout << "Error on discovery request: " << event.message << "\n";
            }

            DEFINE_EVENT_CALLBACK(RetrievedServerListEvent) {
                data.clear();
                data = event.data;
            }

            bool isBlocking() override { return true; }
            bool isCovering() override { return true; }

    };    
    
}
