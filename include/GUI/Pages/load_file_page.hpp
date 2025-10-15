#pragma once

#include <GUI/page.hpp>


namespace SupDef {

    class LoadFilePage : public Page {
        private:
            std::string filename = "";

        public:
            LoadFilePage(PageTypeId pageTypeId_) : Page(pageTypeId_) { }

            void initialize() override {
                // SUBSCRIBE(StartTestReplayEvent)
            }

            void build() override {
                addElement<GuiLabel>(10, 10, "Datei laden");
                addElement<GuiLabel>(40, 20, filename);
                // addElement<GuiFileBrowser>(200, 92, 0.0, 0.0, &filename);

                addButton<StartTestNewGameEvent>(10, 60, 160, 28, "Neu");
                addButton<StartTestSavedGameEvent>(10, 100, 160, 28, "Laden");
                addButton<StartTestReplayEvent>(10, 140, 160, 28, "Replay");

                addButtonEvent<ClosePageEvent>(
                    std::make_tuple(10, 220, 160, 28, "Zurück"),
                    std::make_tuple(pageId)
                );
            }

            DEFINE_EVENT_CALLBACK(ChooseFileEvent) {
                //
            }

            // DEFINE_EVENT_CALLBACK(StartReplayEvent) {
            //     // const char* filters[] = { "*.txt", "*.ini", "*.sdfile" };
            //     // const char* selected = tinyfd_openFileDialog(
            //     //     "Select a file", "", 3, filters, "Allowed files", 0
            //     // );
            //     // if (selected) filename = selected;
            // }

            bool isBlocking() override { return true; }
            bool isCovering() override { return true; }

    };    
    
}
