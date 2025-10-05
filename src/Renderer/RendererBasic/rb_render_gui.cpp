#ifndef RENDERER_BASIC_GUI
#define RENDERER_BASIC_GUI

#include <Renderer/renderer_basic.hpp>


namespace SupDef {

    void RendererBasic::showDebug() {
        if (!debugMode) return;
        std::stringstream ss;

        ss << "Framecount Total:    " << framecountTotal << "\n";
        ss << "           Renderer: " << framecountRenderer << "\n";
        ss << "           Game:     " << framecountGame << "\n";
        ss << "\n";

        auto pos = getMousePos();
        auto posW = getMousePosWorld();
        ss << "Player: " << game->getThisPlayer()->id << "\n";
        ss << "Mouse: " << pos.x << "; " << pos.y << "\n";
        ss << "Mouse world: " << posW.x << "; " << posW.y << "\n";

        // auto guiGame = dynamic_cast<GuiManagerGame*>(gui);
        if (gui) {
            ss << "Selected units amount: " << gui->getSelectionManager()->getSelectedUnits().size() << " [";
            for (auto id : gui->getSelectionManager()->getSelectedUnits()) {
                ss << id << "  ";
            }
            ss << "]\n";

            ss << "Selectable units amount: " << selectables.size() << "\n";
            for (auto [ent, map, bb, xm, ym] : selectables) {
                ss << " -> Entity: " << ent->id << "; map: " << map->id;
                ss << "; x: " << bb->x << "; y: " << bb->y << "; w: " << bb->w << "; h: " << bb->h;
                ss << "; def: " << bb->isDefined;
                ss << "; xm: " << xm << "; ym: " << ym << "\n";
            }
            ss << "\n";

            auto col = game->getCollisionTracker();
            ss << "Collisions: " << col->getCollisionList().size() << "\n";
            for(auto& collision : col->getCollisionList()) {
                ss << "  Entities: " << collision->entityA << " & " << collision->entityB;
                ss << "; framecount: " << collision->frameCount << "; group: " << collision->collisionGroup << "\n";
            }
        }

        ss << "\n--- More Debug Info ---\n";
        ss << game->toPrint << "\n";

        drawLabel(GuiElementStyle::Default, 2, 100, ss.str());
    }

    void RendererBasic::renderGui() {
        if(!gui) return;
        gui->checkIdUniqueness();
        for (const auto& element : gui->getGuiElements()) {
            if (element->embedded) continue;
            drawElement(element.get());
        }
        showDebug();
    }

    void RendererBasic::drawElement(GuiElement* element) {
        assert(element);
        ImGui::PushID(element->guiId.c_str());
        switch (element->type) {
            case GuiElementType::Panel:
                drawPanel(element->style, element->x, element->y, element->width, element->height);
                break;
            case GuiElementType::Label:
                drawLabel(element->style, element->x, element->y, element->text);
                break;
            case GuiElementType::Button:
                drawButton(element->style, element->x, element->y, element->width, element->height, element->text);
                break;
            case GuiElementType::Checkbox:
                drawCheckbox(dynamic_cast<GuiCheckbox*>(element));
                break;
            case GuiElementType::Table:
                drawTable(dynamic_cast<GuiTable*>(element));
                break;
            case GuiElementType::TableComplex:
                drawTableComplex(dynamic_cast<GuiTableComplex*>(element));
                break;
        }
        ImGui::PopID();
        addClickHandling(element);
    }

    void RendererBasic::drawPanel(GuiElementStyle style, float x, float y, float width, float height) {
        ColorData cd(sf::Color::Black, sf::Color::Black, 0.0);
        int innerFrameDistance = 1;
        float frameThickness = 1.0;

        switch (style) {
            case GuiElementStyle::Default:
                cd.fillColor = sf::Color::Blue;
                cd.outlineColor = sf::Color::White;
                innerFrameDistance = 0;
                frameThickness = 1.0;
                break;
            case GuiElementStyle::Test:
                cd.fillColor = sf::Color::Magenta;
                cd.outlineColor = sf::Color::Blue;
                innerFrameDistance = 3;
                frameThickness = 2.0;
                break;
            default:
                break;
        }

        if (innerFrameDistance == 0) {
            cd.outlineThickness = frameThickness;
            drawRect(x, y, width, height, cd);
        } else {
            ColorData outer(cd.fillColor, cd.fillColor, 1.0);
            ColorData inner(cd.fillColor, cd.outlineColor, frameThickness);
            drawRect(x, y, width, height, outer);
            drawRect(x + frameThickness, y + frameThickness,
                width - 2 * frameThickness, height - 2 * frameThickness, inner);
        }
        
        // ImGui::SetCursorPos(ImVec2(x, y));
        // ImGui::BeginChild("##panel", ImVec2(width, height), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
        // ImGui::EndChild();
    } 

    void RendererBasic::drawLabel(GuiElementStyle style, float x, float y, const std::string text) {
        if (!dontSetPosition) ImGui::SetCursorPos(ImVec2(x, y));
        ImGui::Text("%s", text.c_str());
    }   

    void RendererBasic::drawButton(GuiElementStyle style, float x, float y, float width, float height, std::string text) {
        if (!dontSetPosition) ImGui::SetCursorPos(ImVec2(x, y));
        ImGui::Button(text.c_str(), ImVec2(width, height));
    }

    void RendererBasic::drawCheckbox(GuiElementStyle style, float x, float y, std::string text, bool* checked) {
        assert(checked);
        if (!dontSetPosition) ImGui::SetCursorPos(ImVec2(x, y));
        ImGui::Checkbox(text.c_str(), checked);
    }

    void RendererBasic::drawCheckbox(GuiCheckbox* checkbox) {
        assert(checkbox);
        drawCheckbox(checkbox->style, checkbox->x, checkbox->y, checkbox->text, checkbox->isChecked);
    }

    void RendererBasic::drawTable(GuiTable* table) {
        assert(table);

        ImGuiTableFlags flags =
            ImGuiTableFlags_Resizable |
            ImGuiTableFlags_Reorderable |
            ImGuiTableFlags_Hideable |
            ImGuiTableFlags_RowBg |
            ImGuiTableFlags_Borders |
            ImGuiTableFlags_ScrollY |
            ImGuiTableFlags_SizingStretchProp;
            
        if (!dontSetPosition) ImGui::SetCursorPos(ImVec2(table->x, table->y));

        float width = ImGui::GetContentRegionAvail().x;
        if (table->width != 0.0) width = table->width;

        float height = ImGui::GetContentRegionAvail().y;
        if (table->height != 0.0) height = table->height;

        ImVec2 size = ImVec2(width, height);

        if (table->head.size() > 0) {
            if (ImGui::BeginTable(table->guiId.c_str(), table->head.size(), flags, size)) {
                table->distributeColumnWidths(width);
                assert(table->head.size() == table->column_width.size());
                for(int i = 0; i < table->head.size(); i++) {
                    auto col_name = table->head[i].c_str();
                    auto& col_width = table->column_width[i];
                    ImGui::TableSetupColumn(col_name, ImGuiTableColumnFlags_WidthFixed, col_width);
                }

                ImGui::TableHeadersRow();

                // Optional: sortable headers
                // (Read ImGui::TableGetSortSpecs() if you want to implement sorting yourself.)

                // Fill rows
                for (const auto& line : table->data) {
                    assert(line.size() == table->head.size());
                    ImGui::TableNextRow();

                    for(int i = 0; i < line.size(); i++) {
                        ImGui::TableSetColumnIndex(i);
                        ImGui::TextUnformatted(line[i].c_str());
                    }
                }

                ImGui::EndTable();
            }
        }
    }

    void RendererBasic::drawTableComplex(GuiTableComplex* table) {
        assert(table);

        ImGuiTableFlags flags =
            ImGuiTableFlags_Resizable |
            ImGuiTableFlags_Reorderable |
            ImGuiTableFlags_Hideable |
            ImGuiTableFlags_RowBg |
            ImGuiTableFlags_Borders |
            ImGuiTableFlags_ScrollY |
            ImGuiTableFlags_SizingStretchProp;
            
        if (!dontSetPosition) ImGui::SetCursorPos(ImVec2(table->x, table->y));

        float width = ImGui::GetContentRegionAvail().x;
        if (table->width != 0.0) width = table->width;

        float height = ImGui::GetContentRegionAvail().y;
        if (table->height != 0.0) height = table->height;

        ImVec2 size = ImVec2(width, height);

        if (table->head.size() > 0) {
            if (ImGui::BeginTable(table->guiId.c_str(), table->head.size(), flags, size)) {
                table->distributeColumnWidths(width);
                assert(table->head.size() == table->column_width.size());
                for(int i = 0; i < table->head.size(); i++) {
                    auto col_name = table->head[i].c_str();
                    auto& col_width = table->column_width[i];
                    ImGui::TableSetupColumn(col_name, ImGuiTableColumnFlags_WidthFixed, col_width);
                }

                ImGui::TableHeadersRow();
                for (const auto& row : table->rows) {
                    assert(row.size() == table->head.size());
                    ImGui::TableNextRow();
                    for(int i = 0; i < row.size(); i++) {
                        ImGui::TableSetColumnIndex(i);
                        dontSetPosition = true;
                        drawElement(row[i]);
                        dontSetPosition = false;
                    }
                }

                ImGui::EndTable();
            }
        }
    }

    void RendererBasic::addClickHandling(GuiElement* element) {
        assert(element);
        if (element->clickable) {
            auto ptr = static_cast<void*>(element);
            if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) {
                dispatch<GuiButtonClickedEvent>(ptr, MLEFT);
                // std::cout << "Button " << ptr << " left clicked!\n";
            } else if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
                dispatch<GuiButtonClickedEvent>(ptr, MRIGHT);
                // std::cout << "Button " << ptr << " right clicked!\n";
            }
        }
    }

    void RendererBasic::drawCooldownOverlay(float x, float y, float width, float height, Cooldown current, Cooldown max) {
        auto perc = ((double) current) / ((double) max);
        drawCooldownOverlay(x, y, width, height, (float) perc);
    }

    void RendererBasic::drawCooldownOverlay(float x, float y, float width, float height, float perc) {
        sf::Color overlay(37, 150, 190, 150);
        ColorData cd(overlay, overlay, 0);
        drawRect(x, y, width * perc, height, cd);
    }

}

#endif
