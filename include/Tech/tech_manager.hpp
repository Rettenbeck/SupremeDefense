#include <Tech/tech.hpp>
#include <ECS/constants.hpp>
#include <EventDispatcher/include.hpp>

#pragma once


namespace SupDef {

    class TechManager {
        private:
            EventDispatcher* eventDispatcher = nullptr;
            std::unordered_map<TechID, UTech> techs;
            std::unordered_map<EntityID, std::vector<TechID>> ownerMapping;
            std::unordered_map<EntityID, std::vector<TechID>> assigneeMapping;
            TechID nextTechID = 1;

        public:
            std::vector<TechID> complexDependents;

            TechManager(EventDispatcher* eventDispatcher_) : eventDispatcher(eventDispatcher_) {}
            TechManager() {}

            Tech* createTech(TechType techType, json data, DependencyType dependencyType, TechID sourceID) {
                TechID techID = nextTechID++;
                techs.emplace(techID, std::make_unique<Tech>(techID, techType, std::move(data), dependencyType));
                linkTechs(sourceID, techID);
                return techs[techID].get();
            }
            
            Tech* createTech(TechType techType, json data, DependencyType dependencyType) {
                TechID techID = nextTechID++;
                techs.emplace(techID, std::make_unique<Tech>(techID, techType, std::move(data), dependencyType));
                return techs[techID].get();
            }
            
            Tech* createTech(TechType techType, json data) {
                return createTech(techType, data, DependencyType::Independent);
            }
            
            void addOwner(EntityID entity, TechID techID) {
                ownerMapping[entity].push_back(techID);
            }
        
            void addAssignee(EntityID entity, TechID techID) {
                assigneeMapping[entity].push_back(techID);
            }
        
            void removeOwner(EntityID entity) {
                if (ownerMapping.count(entity)) {
                    TechIDs techIDs;
                    for (auto techID : ownerMapping[entity]) {
                        techIDs.push_back(techID);
                    }
                    ownerMapping.erase(entity);
                    for (auto techID : techIDs) {
                        if (isTechOrphaned(techID)) {
                            removeTech(techID);
                        }
                    }
                }
            }
        
            void removeAssignee(EntityID entity) {
                assigneeMapping.erase(entity);
            }
        
            bool isTechOrphaned(TechID techID) {
                for (const auto& [entity, techs] : ownerMapping) {
                    if (std::find(techs.begin(), techs.end(), techID) != techs.end()) {
                        return false;
                    }
                }
                return true;
            }
        
            void removeTech(TechID techID) {
                if (!techs.count(techID)) return;
                auto tech = techs[techID].get();
                
                for (auto& [entity, techs] : ownerMapping) {
                    techs.erase(std::remove(techs.begin(), techs.end(), techID), techs.end());
                }
                for (auto& [entity, techs] : assigneeMapping) {
                    techs.erase(std::remove(techs.begin(), techs.end(), techID), techs.end());
                }

                for (auto dependentID : tech->dependents) {
                    if (techs.count(dependentID)) {
                        auto dependentTech = techs[dependentID].get();
                        dependentTech->removeSource(techID);
        
                        if (dependentTech->dependencyType == DependencyType::RequiresSource && !dependentTech->hasSources()) {
                            removeTech(dependentID);
                        } else if(dependentTech->dependencyType == DependencyType::GameManaged) {
                            complexDependents.push_back(dependentID);
                        }
                    }
                }

                techs.erase(techID);
            }
            
            void onEntityDestroyed(EntityID entity) {
                removeOwner(entity);
                removeAssignee(entity);
            }
            
            void linkTechs(TechID sourceID, TechID dependentID) {
                if (techs.count(sourceID) && techs.count(dependentID)) {
                    techs[sourceID]->addDependent(dependentID);
                    techs[dependentID]->addSource(sourceID);
                }
            }
            
            void unlinkTechs(TechID sourceID, TechID dependentID) {
                if (techs.count(sourceID) && techs.count(dependentID)) {
                    techs[sourceID]->removeDependent(dependentID);
                    techs[dependentID]->removeSource(sourceID);
                }
            }

            void to_json(json& j) const {
                j[ST_TECHS] = json::array();
                for (const auto& [id, tech] : techs) {
                    json techJson;
                    tech->to_json(techJson);
                    j[ST_TECHS].push_back(techJson);
                }
                j[ST_OWNERS] = ownerMapping;
                j[ST_ASSIGNEES] = assigneeMapping;
                j[ST_NEXT_TECH_ID] = nextTechID;
            }
        
            void from_json(const json& j) {
                techs.clear();
                ownerMapping.clear();
                assigneeMapping.clear();
                for (const auto& techJson : j[ST_TECHS]) {
                    //auto tech = Tech::from_json(techJson);
                    auto tech = std::make_unique<Tech>();
                    tech->from_json(techJson);
                    techs.emplace(tech->id, std::move(tech));
                }
                ownerMapping = j.at(ST_OWNERS).get<std::unordered_map<EntityID, std::vector<TechID>>>();
                assigneeMapping = j.at(ST_ASSIGNEES).get<std::unordered_map<EntityID, std::vector<TechID>>>();
                nextTechID = j[ST_NEXT_TECH_ID];
            }

            std::string toStr() {
                std::stringstream ss;
                ss << "TechManager\n";
                ss << "  next ID: " << nextTechID << "\n";
                ss << "  Techs: " << techs.size() << "\n";
                ss << "  Mapping Owners: " << ownerMapping.size() << "\n";
                for(auto& e : ownerMapping) {
                    //ss << "    " << e.first << "; " << e.second << "\n";
                }
                ss << "  Mapping Assignees: " << assigneeMapping.size() << "\n";
                for(auto& e : assigneeMapping) {
                    //ss << "    " << e.first << "; " << e.second << "\n";
                }
                return ss.str();
            }

    };

    using UTechManager = std::unique_ptr<TechManager>;

}
