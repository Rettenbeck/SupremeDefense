#include <GUI/gui_elements.hpp>
#include <Game/game.hpp>

#pragma once


namespace SupDef {

    class GuiManager {
        private:
            GuiElements elements;
            Game* game = nullptr;
            SelectionManager* selectionManager = nullptr;
            unsigned width = 1, height = 1;
        
        public:
            ~GuiManager() {}
        
            void update(float deltaTime) {
                elements.clear();

                showResources(0, 0);
                // elements.push_back(std::make_unique<GuiPanel >(GuiElementStyle::Default, 1200, 10, 150, 50));
                // elements.push_back(std::make_unique<GuiButton>(GuiElementStyle::Default,  800, 10, 150, 50, "Play"));
                // elements.push_back(std::make_unique<GuiButton>(GuiElementStyle::Default, 1000, 10, 150, 50, "Exit"));
            }

            void showResources(unsigned x, unsigned y, unsigned line_distance = 16) {
                auto player = game->getThisPlayer();
                if (!player) return;
                auto resComp = player->getComponent<ResourceComponent>();
                if (!resComp) return;

                unsigned dy = 0;
                for(const auto& [ id , resource ] : resComp->resources) {
                    assert(resource);
                    std::string desc = id;
                    auto res_asset = game->getAssetManager()->getAsset(id);
                    if (res_asset) {
                        auto descComp = res_asset->getComponent<TextComponent>();
                        if (descComp) {
                            desc = descComp->get(SupDef::SX_NAME);
                        }
                    }
                    std::stringstream ss;
                    ss << desc << ": " << resource->amount;
                    if(resource->rate != 0) ss << "; Rate: " << resource->rate;
                    if(resource->capacity != 0) ss << "; Max: " << resource->capacity;

                    elements.push_back(std::make_unique<GuiLabel>(GuiElementStyle::Default, x, y + dy, ss.str()));
                    dy += line_distance;
                }
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

            void setGame(Game* game_) { game = game_; }
            void setSelectionManager(SelectionManager* selectionManager_) { selectionManager = selectionManager_; }
            SelectionManager* getSelectionManager() { return selectionManager; }

    };

    using UGuiManager = std::unique_ptr<GuiManager>;

}
