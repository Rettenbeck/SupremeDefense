#pragma once

#include <GUI/page.hpp>


namespace SupDef {

    class ModListPage : public Page {
        private:
            std::vector<std::string> data;

        public:
            ModListPage(PageTypeId pageTypeId_) : Page(pageTypeId_) { }

            void initialize() override {
                SUBSCRIBE(RetrieveModListAnswerEvent)
            }

            void build() override {
                addElement<GuiLabel>(10, 10, "Mods");

                addButton<RetrieveModListEvent>(200, 60, 160, 28, "Refresh");

                auto ptr_table = addElement<GuiTableComplex>(200, 92, 0.0, 0.0);
                fillTableData(ptr_table);
                
                addClickableEvent<GuiButton, ClosePageEvent>(
                    std::make_tuple(10,  60, 160, 28, "Zurück"),
                    std::make_tuple(pageId)
                );
            }

            void fillTableData(GuiTableComplex* table) {
                assert(table);
                TableLine head = {"Mod", "Beschreibung", "Aktiv", "Default", "Pfad"};
                table->column_width_perc = {1, 5, 0.3, 1, 2};
                table->setHead(head);
                table->rows.clear();
                if (data.empty()) return;
                assert(!table->head.empty());

                std::vector<std::string> line;
                std::vector<std::vector<std::string>> data_tabled;
                for(auto str : data) {
                    assert(line.size() <= table->head.size());
                    if (line.size() == table->head.size()) {
                        data_tabled.push_back(line);
                        line.clear();
                    }
                    line.push_back(str);
                }
                if (line.size() > 0) {
                    while (line.size() < table->head.size()) line.push_back("");
                    data_tabled.push_back(line);
                }
                insertRowElements(table, data_tabled);
            }

            void insertRowElements(GuiTableComplex* table, std::vector<std::vector<std::string>>& data_tabled) {
                GuiElementRow row;
                for(auto& line : data_tabled) {
                    row.clear();
                    for(int i = 0; i < line.size(); i++) {
                        auto& dat = line[i];
                        GuiElement* element = nullptr;
                        if (i == 2) { // Checkbox instead of text
                            bool checked = false; if (dat == "y") checked = true;
                            element = addElement<GuiCheckbox>(0.0, 0.0, "", checked);
                        } else {
                            element = addElement<GuiLabel>(0.0, 0.0, dat);
                        }
                        assert(element);
                        element->embedded = true;
                        row.push_back(element);
                    }
                    table->rows.push_back(row);
                }
            }

            DEFINE_EVENT_CALLBACK(RetrieveModListAnswerEvent) {
                data.clear();
                data = event.data;
            }

            bool isBlocking() override { return true; }
            bool isCovering() override { return true; }

    };    
    
}
