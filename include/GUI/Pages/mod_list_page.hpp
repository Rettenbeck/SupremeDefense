#pragma once

#include <GUI/page.hpp>
#include <Game/mod_data.hpp>


namespace SupDef {

    class ModListPage : public Page {
        private:
            Mods* mods = nullptr;
            bool t_hoverable = true, t_clickable = true;

        public:
            ModListPage(PageTypeId pageTypeId_) : Page(pageTypeId_) { }

            ~ModListPage() {
                dispatch<SaveModListEvent>();
            }

            void initialize() override {
                SUBSCRIBE(RequestModListAnswerEvent)
                dispatch<RequestModListEvent>();
            }

            void build() override {
                addElement<GuiLabel>(10, 10, "Mods");

                addButton<BuildModListEvent>(200, 60, 160, 28, "Refresh");

                addElement<GuiCheckbox>(400, 60, "Hoverable", &(t_hoverable));
                addElement<GuiCheckbox>(500, 60, "Clickable", &(t_clickable));

                auto ptr_table = addElement<GuiTable>(200, 92, 0.0, 0.0);
                fillTableData(ptr_table);
                
                addClickableEvent<GuiButton, ClosePageEvent>(
                    std::make_tuple(10,  60, 160, 28, "Zurück"),
                    std::make_tuple(pageId)
                );
            }

            void fillTableData(GuiTable* table) {
                assert(table);
                TableLine head = {"Mod", "Beschreibung", "Aktiv", "Exklusiv", "Default", "Pfad"};
                table->column_width_perc = {1, 5, 0.3, 0.3, 1, 2};
                table->setHead(head);
                table->rows.clear();
                if (!mods) return;
                if (mods->empty()) return;
                assert(!table->head.empty());

                for(auto& mod : (*mods)) {
                    modToRow(table, mod.get());
                }
                table->distributeGuiIds();
                table->hoverable = t_hoverable;
                table->clickable = t_clickable;
            }

            void modToRow(GuiTable* table, ModData* mod) {
                assert(table);
                assert(mod);
                GuiElementRow row;

                auto exclusive = mod->exclusive;
                addElementToRow(row, addElement<GuiLabel>(mod->getName()));
                addElementToRow(row, addElement<GuiLabel>(mod->desc->get()));
                addElementToRow(row, addElement<GuiCheckbox>(&(mod->active)));
                addElementToRow(row, addElement<GuiCheckboxReadOnly>(&(mod->exclusive)));
                addElementToRow(row, addElement<GuiLabel>(mod->defaultname));
                addElementToRow(row, addElement<GuiLabel>(mod->path));
                table->rows.push_back(row);
                table->checkeds.push_back(&(mod->active));
            }

            DEFINE_EVENT_CALLBACK(RequestModListAnswerEvent) {
                mods = event.mods;
            }

            bool isBlocking() override { return true; }
            bool isCovering() override { return true; }

    };    
    
}
