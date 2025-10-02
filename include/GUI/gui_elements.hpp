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

        bool setHead(TableLine head_) {
            if (!data.empty()) return false;
            head = head_;
            return true;
        }

        void clear() { data.clear(); }

        void appendData(std::vector<std::string> data_) {
            assert(!head.empty());  // Headerline must be filled
            TableLine tmp;
            for(auto& d : data_) {
                if (tmp.size() >= head.size()) {
                    data.push_back(tmp);
                    tmp.clear();
                }
                tmp.push_back(d);
            }
            if (tmp.size() > 0) data.push_back(tmp);
        }

        void appendData(std::vector<std::vector<std::string>> data_) {
            for(auto& line : data_) data.push_back(line);
        }

    };

}
