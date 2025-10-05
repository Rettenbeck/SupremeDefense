#pragma once

#include <GUI/gui_element.hpp>


namespace SupDef {

    struct GuiButton : public GuiElement {
        bool isPressed = false;
        GuiButton(float x, float y, float width, float height, const std::string& text)
        : GuiElement(GuiElementType::Button, GuiElementStyle::Default, x, y, width, height, text) {
            clickable = true;
        }
    };
    
    struct GuiLabel : public GuiElement {
        GuiLabel(float x, float y, const std::string& text)
        : GuiElement(GuiElementType::Label, GuiElementStyle::Default, x, y, 0.0, 0.0, text) {
            clickable = false;
        }
        GuiLabel(const std::string& text)
        : GuiElement(GuiElementType::Label, GuiElementStyle::Default, 0.0, 0.0, 0.0, 0.0, text) {
            clickable = false;
        }
    };
    
    struct GuiPanel : public GuiElement {
        GuiPanel(float x, float y, float width, float height)
        : GuiElement(GuiElementType::Panel, GuiElementStyle::Default, x, y, width, height) {
            clickable = true;
        }
    };

    struct GuiInput : public GuiElement {
        std::string value = "";
        GuiInput(float x, float y, const std::string& text)
        : GuiElement(GuiElementType::Input, GuiElementStyle::Default, x, y, 0.0, 0.0, text) {
            clickable = false;
        }
    };

    struct GuiCheckbox : public GuiElement {
        bool isPressed = false, isReadOnly = false;
        bool* checkedPtr;
        std::unique_ptr<bool> checkedPtrReadOnly;

        GuiCheckbox(float x, float y, const std::string& text, bool* checked)
        : GuiElement(GuiElementType::Checkbox, GuiElementStyle::Default, x, y, 0.0, 0.0, text) , checkedPtr(checked) {
            clickable = true; }

        GuiCheckbox(float x, float y, bool* checked)
        : GuiElement(GuiElementType::Checkbox, GuiElementStyle::Default, x, y, 0.0, 0.0, "") , checkedPtr(checked) {
            clickable = true; }

        GuiCheckbox(const std::string& text, bool* checked)
        : GuiElement(GuiElementType::Checkbox, GuiElementStyle::Default, 0.0, 0.0, 0.0, 0.0, text) , checkedPtr(checked) {
            clickable = true; }

        GuiCheckbox(bool* checked)
        : GuiElement(GuiElementType::Checkbox, GuiElementStyle::Default, 0.0, 0.0, 0.0, 0.0, "") , checkedPtr(checked) {
            clickable = true; }

        bool* getCheckedPtr() {
            if (isReadOnly) {
                checkedPtrReadOnly = std::make_unique<bool>(*checkedPtr);
                return checkedPtrReadOnly.get();
            } else {
                return checkedPtr;
            }
        }
    };
    
    struct GuiCheckboxReadOnly : public GuiCheckbox {
        GuiCheckboxReadOnly(bool* checked) : GuiCheckbox(checked) {
            isReadOnly = true; }
    };

    struct GuiTable : public GuiElement {
        std::vector<float> column_width;
        std::vector<float> column_width_perc;
        TableLine head;
        TableData data;

        GuiTable(float x, float y, float width, float height)
        : GuiElement(GuiElementType::Table, GuiElementStyle::Default, x, y, width, height) {
            clickable = false;
        }

        bool setHead(TableLine head_) {
            // if (!data.empty()) return false;
            head = head_;
            return true;
        }

        void distributeColumnWidths(float width_) {
            width = width_;
            distributeColumnWidths();
        }

        void distributeColumnWidths() {
            if (width == 0.0) return;
            assert(!head.empty());
            if (column_width_perc.empty()) {
                distributeColumnWidthsEqually();
            } else {
                assert(column_width_perc.size() == head.size());
                column_width.clear();
                float total_perc = 0.0;
                for(int i = 0; i < column_width_perc.size(); i++) total_perc += column_width_perc[i];
                assert(total_perc != 0.0);
                for(int i = 0; i < column_width_perc.size(); i++) {
                    column_width_perc[i] /= total_perc;
                }
                for(int i = 0; i < column_width_perc.size(); i++) {
                    column_width.push_back(width * column_width_perc[i]);
                }
            }
        }

        void distributeColumnWidthsEqually() {
            if (width == 0.0) return;
            assert(!head.empty());
            auto width_per_column = width / ((float) head.size());
            column_width.clear();
            for(int i = 0; i < head.size(); i++) column_width.push_back(width_per_column);
        }

        void checkColumnWidths() {
            // if (head.empty()) return;
            if (column_width.size() == head.size()) {
                //
            } else if (column_width.size() < head.size()) {
                distributeColumnWidths();
            }
            while(column_width.size() > head.size()) {
                column_width.pop_back();
            }
        }

        void clear() { data.clear(); }

        void appendData(std::vector<std::string> data_) {
            assert(!head.empty());
            TableLine tmp;
            for(auto& d : data_) {
                if (tmp.size() >= head.size()) {
                    data.push_back(tmp);
                    tmp.clear();
                }
                tmp.push_back(d);
            }
            if (tmp.size() > 0) {
                while(tmp.size() < head.size()) tmp.push_back("");
                data.push_back(tmp);
            }
        }

        void appendData(std::vector<std::vector<std::string>> data_) {
            for(auto& line : data_) data.push_back(line);
        }

    };

    struct GuiTableComplex : public GuiTable {
        GuiElementRows rows;

        GuiTableComplex(float x, float y, float width, float height)
        : GuiTable(x, y, width, height) {
            clickable = false;
            type = GuiElementType::TableComplex;
        }
        
        void appendData(std::vector<std::string> data_) = delete;
        void appendData() = delete;

    };

}
