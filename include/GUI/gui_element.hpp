#pragma once

#include <Util/basic.hpp>
#include <GUI/constants.hpp>


namespace SupDef {

    enum class GuiElementType  { Button, Label, Panel, Input, Table };
    enum class GuiElementStyle { Default, Test };

    struct GuiElement {
        GuiElementType type;
        GuiElementStyle style = GuiElementStyle::Default;
        std::string text;
        float x, y, width, height;
        bool clickable = true;
        char ipBuffer[64] = "";

        GuiElement(GuiElementType type, GuiElementStyle style, float x, float y, float width, float height, std::string text = "")
        : type(type), style(style), x(x), y(y), width(width), height(height), text(std::move(text)) {}

        virtual ~GuiElement() = default;
    };

    using UGuiElement = std::unique_ptr<GuiElement>;
    using GuiElements = std::vector<UGuiElement>;
    using PGuiElements = std::vector<GuiElement*>;

}
