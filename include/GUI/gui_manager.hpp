#include <GUI/gui_elements.hpp>
#include <Game/game.hpp>

#pragma once


namespace SupDef {

    using MouseClick = bool;
    const MouseClick MLEFT = false;
    const MouseClick MRIGHT = true;

    // using GUI_TechMapEntry = std::tuple<EntityID, EntityID, ActiveTechComponent*, CommandComponent*>;
    using GUI_TechMapEntry = std::tuple<EntityID, EntityID, ActiveTechComponent*>;
    using GUI_TechMap = std::vector<GUI_TechMapEntry>;
    using GUI_ElementMap = std::unordered_map<GuiElement*, std::tuple<EntityID, EntityID>>;

    class GuiManager : public Listener {
        private:
            GuiElements elements;
            Game* game = nullptr;
            SelectionManager* selectionManager = nullptr;
            GUI_TechMap techMap;
            GUI_ElementMap elementMap;
            unsigned width = 1, height = 1;
            int mouseX = 0, mouseY = 0;
            std::string info = "";
        
        public:
            ~GuiManager() {}
        
            void update(float deltaTime) {
                info.clear();
                elements.clear();

                showResources(0, 0);
                buildGuiForUnit();
                if (!info.empty())
                    elements.push_back(std::make_unique<GuiLabel>(GuiElementStyle::Default, 1000, 10, info));
                // elements.push_back(std::make_unique<GuiPanel >(GuiElementStyle::Default, 1200, 10, 150, 50));
            }

            void buildGuiForUnit() {
                buildGuiForUnit(5, height - 32 * 4, 64, 32, 1, 1, 5);
            }

            void buildGuiForUnit(int x, int y, int bw, int bh, int sx, int sy, int cols) {
                buildTechMap();
                elementMap.clear();

                int cx = x, cy = y, cc = 0;
                std::string desc = "";
                std::stringstream ss;
                for (const auto& [id, techID, active] : techMap) {
                    ss << "TechMap: " << id << "; tech: " << techID << "; active? " << active << "\n";
                    auto command = game->getCommandFromActiveTech(active);
                    auto txtComp = command->getComponent<TextComponent>();
                    if (txtComp) {
                        desc = txtComp->get(SupDef::SX_NAME);
                    } else {
                        desc = active->commandID;
                    }
                    ss << "  Desc: " << desc << "\n";
                    auto button = std::make_unique<GuiButton>(GuiElementStyle::Default,  cx, cy, bw, bh, desc);
                    elementMap[button.get()] = {id, techID};
                    elements.push_back(std::move(button));
                    cx += bw + 1; cc++;
                    if (cc >= cols) {
                        cx = x;
                        cy += bh + 1;
                    }
                }
                std::stringstream total;
                total << info << ss.str();
                info = total.str();
            }

            void buildTechMap() {
                techMap.clear();
                std::stringstream ss;
                auto selected = selectionManager->getSelectedUnits();
                if (selected.empty()) return;

                auto& assigneeToTechs = game->getAssigneeToTechs();
                auto entities = game->getEntityManager()->getEntities(selected);
                ss << "Entities selected: " << entities.size() << "\n";
                for (auto entity : entities) {
                    auto techIDs = assigneeToTechs[entity->id];
                    auto techs = game->getEntityManager()->getEntities(techIDs);
                    ss << "  Entity: " << entity->id << "; tech size: " << techs.size() << "\n";
                    for (auto tech : techs) {
                        auto active = tech->getComponent<ActiveTechComponent>();
                        ss << "    Tech: " << tech->id << "; active? " << active << "\n";
                        if (active) {
                            auto command = game->getCommandFromActiveTech(active);
                            ss << "      Command: " << active->commandID << "\n";
                            if (command) {
                                ss << "        Emplaced!" << active->commandID << "\n";
                                techMap.emplace_back(entity->id, tech->id, active);
                            }
                        }
                    }
                }

                info = ss.str();
                //filterTechMapForSharedTechs();
            }

            void filterTechMapForSharedTechs() {
                auto tmpMap = techMap;
                techMap.clear();

                // Step 1: Collect all unique persons
                std::unordered_set<CommandID> allEntities;
                for (const auto& [id, techID, active] : tmpMap) {
                    allEntities.insert(active->commandID);
                }
                size_t entityCount = allEntities.size();

                // Step 2: Count how many unique persons are assigned to each equipment
                std::unordered_map<CommandID, std::unordered_set<EntityID>> commandToEntity;
                for (const auto& [id, techID, active] : tmpMap) {
                    commandToEntity[active->commandID].insert(id);
                }

                // Step 3: Find the equipment assigned to ALL persons
                std::unordered_set<CommandID> sharedCommands;
                for (const auto& [commandID, ids] : commandToEntity) {
                    if (ids.size() == entityCount) {
                        sharedCommands.insert(commandID);
                    }
                }

                // Step 4: Filter original assignments to keep only those with shared equipment
                for (const auto& [id, techID, active] : tmpMap) {
                    if (sharedCommands.count(active->commandID)) {
                        techMap.emplace_back(id, techID, active);
                    }
                }
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

            void handleClickOnGui(GuiElement* element, MouseClick button) {
                if (!element) return;
                auto it = elementMap.find(element);
                assert(it != elementMap.end());
                auto [id, techID] = (*it).second;
                auto tech = game->getEntityManager()->getEntity(techID);
                assert(tech);
                auto active = tech->getComponent<ActiveTechComponent>();
                assert(active);
                assert(globalDispatcher);
                globalDispatcher->dispatch<TriggerCommandEvent>(id, techID, json());
            }
        
            const GuiElements& getGuiElements() const { return elements; }

            void resize(unsigned width_, unsigned height_) {
                width = width_; height = height_;
                std::cout << "Resized to " << width << " & " << height << "\n";
            }

            void setMousePos(int x, int y) { mouseX = x; mouseY = y; }
            void setGame(Game* game_) { game = game_; }
            void setSelectionManager(SelectionManager* selectionManager_) { selectionManager = selectionManager_; }
            SelectionManager* getSelectionManager() { return selectionManager; }

    };

    using UGuiManager = std::unique_ptr<GuiManager>;

}
