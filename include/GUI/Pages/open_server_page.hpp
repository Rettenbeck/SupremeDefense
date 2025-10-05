#pragma once

#include <GUI/page.hpp>


namespace SupDef {

    class OpenServerPage : public Page {
        public:
            OpenServerPage(PageTypeId pageTypeId_) : Page(pageTypeId_) { }

            ~OpenServerPage() {
                dispatch<RequestOpenServerCloseEvent>();
            }

            void initialize() override {
                SUBSCRIBE(RequestServerOpenAnswerEvent)
                dispatch<RequestServerOpenEvent>();
            }

            void build() override {
                addElement<GuiLabel>(10, 10, "Server aufmachen");

                addButtonEvent<ClosePageEvent>(
                    std::make_tuple(10,  60, 160, 28, "Zurück"),
                    std::make_tuple(pageId)
                );
            }

            DEFINE_EVENT_CALLBACK(RequestServerOpenAnswerEvent) {
                if (!event.ok) {
                    // TODO: Message for the user?
                    std::cout << "Server could not be opened: " << event.message << "\n";
                    close();
                    return;
                }
            }

            bool isBlocking() override { return true; }
            bool isCovering() override { return true; }

    };    
    
}
