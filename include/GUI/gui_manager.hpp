#include <GUI/gui_elements.hpp>

#pragma once


namespace SupDef {

    class GuiManager {
        private:
            GuiElements elements;
            unsigned width = 1, height = 1;
        
        public:
            ~GuiManager() {}
        
            void update(float deltaTime) {
                elements.clear();
        
                elements.push_back(std::make_unique<GuiPanel>(GuiElementStyle::Default, 450,  50, 80, 70));
                elements.push_back(std::make_unique<GuiPanel>(GuiElementStyle::Test, 450, 250, 80, 70));
                //elements.push_back(std::make_unique<GuiLabel>(460, 60, "Main Menu"));
                elements.push_back(std::make_unique<GuiButton>(GuiElementStyle::Default, 500, 200, 150, 50, "Play"));
                elements.push_back(std::make_unique<GuiButton>(GuiElementStyle::Default, 500, 300, 150, 50, "Exit"));
            }
        
            const PGuiElements getGuiElements() const {
                PGuiElements tmp;
                for(auto& g : elements) {
                    tmp.push_back(g.get());
                }
                return tmp;
            }

            void resize(unsigned width_, unsigned height_) {
                width = width_; height = height_;
                std::cout << "Resized to " << width << " & " << height << "\n";
            }

    };

    using UGuiManager = std::unique_ptr<GuiManager>;

}
