#pragma once

#include <GUI/page.hpp>


namespace SupDef {

    using GUI_TechMapEntry = std::tuple<EntityID, EntityID, ActiveTechComponent*>;
    using GUI_TechMap = std::vector<GUI_TechMapEntry>;
    using GUI_ElementMap = std::unordered_map<GuiElement*, std::tuple<EntityID, EntityID>>;

    class GamePage : public Page {
        private:
            GUI_TechMap techMap;
            GUI_ElementMap elementMap;

        public:
            GamePage(PageTypeId pageTypeId_) : Page(pageTypeId_) { }

            void initialize() override {
                SUBSCRIBE(GuiButtonClickedEvent);
                SUBSCRIBE(GameInteractionEvent);
                SUBSCRIBE(GameInteractionMovementEvent);
            }

            void build() override {
                assert(guiManager);
                assert(game);
                showResources(0, 0);
                buildGuiForUnit();
                if (!guiManager->info.empty()) guiManager->add(std::make_unique<GuiLabel>(1000, 10, guiManager->info));
            }

            void buildGuiForUnit() {
                buildGuiForUnit(5, guiManager->getHeight() - 32 * 4, 96, 32, 1, 1, 5);
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
                    auto button = std::make_unique<GuiButton>(cx, cy, bw, bh, desc);
                    elementMap[button.get()] = {id, techID};
                    guiManager->add(std::move(button));
                    cx += bw + 1; cc++;
                    if (cc >= cols) {
                        cx = x;
                        cy += bh + 1;
                    }
                }
                std::stringstream total;
                total << guiManager->info << ss.str();
                guiManager->info = total.str();
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
                    auto techAssignments = assigneeToTechs[entity->id];
                    EntityIDs techIDs;
                    for (auto [techID, persistence] : techAssignments) techIDs.push_back(techID);
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

                guiManager->info = ss.str();
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

                    guiManager->add(std::make_unique<GuiLabel>(x, y + dy, ss.str()));
                    dy += line_distance;
                }
            }

            void handleClickOnGui(GuiElement* element, MouseClick button, json data) {
                std::cout << "Command to trigger 1\n";
                if (!element) return;
                std::cout << "Command to trigger 2\n";
                // if (button == MLEFT) if (handleButton(element)) return;
                auto it = elementMap.find(element);
                assert(it != elementMap.end());
                auto [id, techID] = (*it).second;
                auto tech = game->getEntityManager()->getEntity(techID);
                assert(tech);
                auto active = tech->getComponent<ActiveTechComponent>();
                assert(active);
                std::cout << "Command triggered\n";
                dispatch<TriggerCommandEvent>(id, techID, data);
            }
            
            void handleClickMove(json j) {
                for(auto& [element, tuple] : elementMap) {
                    auto techID = std::get<1>(tuple);
                    auto tech = game->getEntityManager()->getEntity(techID);
                    auto active = tech->getComponent<ActiveTechComponent>();
                    if (active) {
                        auto asset = game->getAssetFromCommand(active->commandID, j);
                        if (asset) {
                            if (asset->hasComponent<MoveCommandComponent>()) {
                                handleClickOnGui(element, MLEFT, j);
                                return;
                            }
                        }
                    }
                }
            }

            bool isInteractionAllowed() {
                return !getBlocked();
            }

            DEFINE_EVENT_CALLBACK(GuiButtonClickedEvent) {
                if (!event.element) return;
                auto guiElement = static_cast<GuiElement*>(event.element);
                handleClickOnGui(guiElement, event.mouseClick, json());
            }

            DEFINE_EVENT_CALLBACK(GameInteractionEvent) {
                if (!isInteractionAllowed()) return;
                event.call();
            }

            DEFINE_EVENT_CALLBACK(GameInteractionMovementEvent) {
                if (!isInteractionAllowed()) return;
                handleClickMove(event.j);
            }

            bool isBlocking() override { return true; }
            bool isCovering() override { return false; }

    };    
    
}
