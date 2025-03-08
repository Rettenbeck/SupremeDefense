#include <GUI/gui_element.hpp>

#pragma once


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

}
