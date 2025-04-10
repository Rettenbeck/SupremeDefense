#pragma once

#include <GUI/gui_element.hpp>


namespace SupDef {

    struct GuiButton : public GuiElement {
        bool isPressed = false;
    
        GuiButton(GuiElementStyle style, float x, float y, float width, float height, const std::string& text)
        : GuiElement(GuiElementType::Button, style, x, y, width, height, text) {
            clickable = true;
        }
    };
    
    struct GuiLabel : public GuiElement {
        GuiLabel(GuiElementStyle style, float x, float y, const std::string& text)
        : GuiElement(GuiElementType::Label, style, x, y, 0, 0, text) {
            clickable = false;
        }
    };
    
    struct GuiPanel : public GuiElement {
        GuiPanel(GuiElementStyle style, float x, float y, float width, float height)
        : GuiElement(GuiElementType::Panel, style, x, y, width, height) {
            clickable = true;
        }
    };

    struct GuiInput : public GuiElement {
        std::string value = "";
        GuiInput(GuiElementStyle style, float x, float y, const std::string& text)
        : GuiElement(GuiElementType::Input, style, x, y, 0, 0, text) {
            clickable = false;
        }
    };

    struct GuiTable : public GuiElement {
        TableLine head;
        TableData data;
        GuiTable(GuiElementStyle style, float x, float y, float width, float height)
        : GuiElement(GuiElementType::Table, style, x, y, width, height) {
            clickable = false;
        }
    };

}
