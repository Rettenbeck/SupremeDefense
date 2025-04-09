#include <GUI/gui_elements.hpp>
#include <Game/game.hpp>

#pragma once


namespace SupDef {

    using MouseClick = bool;
    const MouseClick MLEFT = false;
    const MouseClick MRIGHT = true;

    class GuiManager : public Listener {
        protected:
            using GuiMemberFunc = void (GuiManager::*)();
            Game* game = nullptr;
            GuiElements elements;
            std::unordered_map<int, GuiMemberFunc> clickableMap;
            std::unordered_map<int, GuiInput*> inputMap;
            unsigned width = 1, height = 1;
            int mouseX = 0, mouseY = 0;
            std::string info = "";
        
        public:
            virtual ~GuiManager() {}
        
            virtual void update(float deltaTime) {
                info.clear();
                elements.clear();
                clickableMap.clear();
                inputMap.clear();
            }

            GuiElement* getGuiInSpot(int x, int y) {
                for (auto it = elements.rbegin(); it != elements.rend(); it++) {
                    auto el = (*it).get();
                    float x1 = el->x, y1 = el->y, x2 = x1 + el->width, y2 = y1 + el->height;
                    if (x >= x1 && x <= x2 && y >= y1 && y <= y2) {
                        return el;
                    }
                }
                return nullptr;
            }

            void add(UGuiElement element) {
                elements.push_back(std::move(element));
            }

            void addInput(UGuiElement element, int key) {
                auto ptr_input = dynamic_cast<GuiInput*>(element.get());
                assert(ptr_input);
                inputMap[key] = ptr_input;
                add(std::move(element));
            }

            void addClickable(UGuiElement element, GuiMemberFunc func) {
                clickableMap[elements.size()] = func;
                add(std::move(element));
            }

            void setInput(int key, std::string value) {
                assert(inputMap.count(key));
                inputMap[key]->value = value;
            }

            std::string getInput(int key) {
                assert(inputMap.count(key));
                return inputMap[key]->value;
            }

            bool handleButton(int index) {
                assert(globalDispatcher);
                if (!clickableMap.count(index)) return false;
                (this->*clickableMap[index])();
                return true;
            }

            bool handleButton(GuiElement* element) {
                if (!element) return false;
                if (elements.size() == 0) return false;
                for (int i = 0; i < elements.size(); i++) {
                    if (elements[i].get() == element) return handleButton(i);
                }
                return true;
            }

            virtual void handleClickOnGui(GuiElement* element, MouseClick button, json data) {}
        
            virtual void handleClickOnGui(GuiElement* element, MouseClick button) {
                handleClickOnGui(element, button, json());
            }
        
            virtual void handleClickMove(json& j) { }

            const GuiElements& getGuiElements() const { return elements; }

            void resize(unsigned width_, unsigned height_) {
                width = width_; height = height_;
                // std::cout << "Resized to " << width << " & " << height << "\n";
            }

            void setMousePos(int x, int y) { mouseX = x; mouseY = y; }

    };

    using UGuiManager = std::unique_ptr<GuiManager>;

}
