#pragma once

#include <GUI/page.hpp>
#include <Game/mod_data.hpp>


namespace SupDef {

    class ModListPage : public Page {
        public:
            ModListPage(PageTypeId pageTypeId_) : Page(pageTypeId_) { }

            ~ModListPage() {
                assert(services);
                services->assetService->saveMods();
            }

            void initialize() override {
                assert(services);
                services->assetService->buildModList();
                SUBSCRIBE(BuildModListEvent)
            }

            void build() override {
                addElement<GuiLabel>(10, 10, "Mods");
                addButton<BuildModListEvent>(200, 60, 160, 28, "Refresh");
                fillTableData(addElement<GuiTable>(200, 92, 0.0, 0.0));
                
                addClickableEvent<GuiButton, ClosePageEvent>(
                    std::make_tuple(10,  60, 160, 28, "Zurück"),
                    std::make_tuple(pageId)
                );
            }

            void fillTableData(GuiTable* table) {
                assert(services);
                assert(table);
                TableLine head = {"Mod", "Beschreibung", "Aktiv", "Exklusiv", "Default", "Pfad"};
                table->column_width_perc = {1, 5, 0.3, 0.3, 1, 2};
                table->setHead(head);
                table->rows.clear();
                auto mods = &(services->assetService->getModList());
                if (!mods) return;
                if (mods->empty()) return;
                assert(!table->head.empty());

                for(auto& mod : (*mods)) {
                    modToRow(table, mod.get());
                }
                table->distributeGuiIds();
                table->hoverable = true;
                table->clickable = true;
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

            DEFINE_EVENT_CALLBACK(BuildModListEvent) {
                services->assetService->buildModList();
            }

            bool isBlocking() override { return true; }
            bool isCovering() override { return true; }

    };    
    
}
