#include <Renderer/renderer_basic.hpp>

#pragma once


namespace SupDef {

    void RendererBasic::showDebug() {
        if (!debugMode) return;
        std::stringstream ss;

        auto pos = getMousePos();
        auto posW = getMousePosWorld();
        ss << "Mouse: " << pos.x << "; " << pos.y << "\n";
        ss << "Mouse world: " << posW.x << "; " << posW.y << "\n";

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

        drawLabel(GuiElementStyle::Default, 2, 100, ss.str());
    }

    void RendererBasic::renderGui() {
        if(!gui) return;
        for (const auto& element : gui->getGuiElements()) {
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
            }
        }
        showDebug();
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
        ImGui::SetCursorPos(ImVec2(x, y));
        ImGui::Text("%s", text.c_str());
    }   

    void RendererBasic::drawButton(GuiElementStyle style, float x, float y, float width, float height, std::string text) {
        ImGui::SetCursorPos(ImVec2(x, y));
        if (ImGui::Button(text.c_str(), ImVec2(width, height))) {
            std::cout << "Button '" << text << "' clicked!\n";
        }
    }

}
