#pragma once

#include <GUI/gui_elements.hpp>
#include <Game/game.hpp>
#include <App/layers/network_layer.hpp>


namespace SupDef {

    using MouseClick = bool;
    const MouseClick MLEFT = false;
    const MouseClick MRIGHT = true;

    class GuiManager : public Listener {
        protected:
            GuiElements elements;
            std::unordered_map<int, GuiMemberFunc> clickableMap;
            std::unordered_map<int, GuiInput*> inputMap;
            unsigned width = 1, height = 1;
            int mouseX = 0, mouseY = 0;

            SelectionManager* selectionManager = nullptr;
            bool isGameBlocked = false;
        
        public:
            void initialize() {
                SUBSCRIBE(GuiButtonClickedEvent)
            }

            void update(float deltaTime) {
                clear();
            }

            GuiElement* getGuiInSpot(int x, int y) {
                for (auto it = elements.rbegin(); it != elements.rend(); it++) {
                    auto el = (*it).get();
                    if (el->blocked) continue;
                    float x1 = el->x, y1 = el->y, x2 = x1 + el->width, y2 = y1 + el->height;
                    if (x >= x1 && x <= x2 && y >= y1 && y <= y2) {
                        return el;
                    }
                }
                return nullptr;
            }

            GuiElement* add(UGuiElement element) {
                assert(element);
                auto ptr = element.get();
                elements.push_back(std::move(element));
                return ptr;
            }

            GuiInput* addInput(UGuiElement element, int key) {
                assert(element);
                auto ptr_input = dynamic_cast<GuiInput*>(element.get());
                assert(ptr_input);
                inputMap[key] = ptr_input;
                add(std::move(element));
                return ptr_input;
            }

            GuiElement* addClickable(UGuiElement element, GuiMemberFunc func) {
                assert(element);
                auto ptr = element.get();
                clickableMap[elements.size()] = func;
                add(std::move(element));
                return ptr;
            }

            template<typename T>
            GuiElement* addClickable(UGuiElement element) {
                assert(element);
                auto ptr = element.get();
                auto callback = [&](){ dispatch<T>(); };
                clickableMap[elements.size()] = callback;
                add(std::move(element));
                return ptr;
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
                if (!clickableMap.count(index)) return false;
                (clickableMap[index])();
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

            DEFINE_EVENT_CALLBACK(GuiButtonClickedEvent) {
                if (event.mouseClick == MRIGHT) return;
                if (!event.element) return;
                auto guiElement = static_cast<GuiElement*>(event.element);
                handleButton(guiElement);
            }

            void clear() {
                isGameBlocked = false;
                info.clear();
                elements.clear();
                clickableMap.clear();
                inputMap.clear();
            }

            void setElementsUnreactive() {
                for (auto& element : elements) {
                    element->blocked = true;
                }
            }

            void handleClickOnGui(GuiElement* element, MouseClick button, json data) {
                if (button == MLEFT) if (handleButton(element)) return;
            }
        
            void handleClickOnGui(GuiElement* element, MouseClick button) {
                handleClickOnGui(element, button, json());
            }
        
            virtual void handleClickMove(json& j) { }

            const GuiElements& getGuiElements() const { return elements; }

            void resize(unsigned width_, unsigned height_) {
                width = width_; height = height_;
            }

            void setMousePos(int x, int y) { mouseX = x; mouseY = y; }

            void setSelectionManager(SelectionManager* selectionManager_) { selectionManager = selectionManager_; }
            SelectionManager* getSelectionManager() { return selectionManager; }

            unsigned getWidth () { return width ; }
            unsigned getHeight() { return height; }

            std::string info = "";
            
    };

    using UGuiManager = std::unique_ptr<GuiManager>;

}
