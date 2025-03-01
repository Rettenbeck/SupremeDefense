#include <Util/basic.hpp>
#include <Tech/constants.hpp>
#include <Tech/tech_type.hpp>
#include <Tech/dependency_type.hpp>
#include <nlohmann/include.hpp>

#pragma once


namespace SupDef {

    class Tech {
        public:
            TechID id = NO_TECH_ID;
            //bool isDead = false;
            TechType techType = TechType::None;
            json data;

            std::unordered_set<int> sources;
            std::unordered_set<int> dependents;
            DependencyType dependencyType = DependencyType::Independent;
            
            Tech() {}
            //Tech() : id(NO_TECH_ID), techType(TechType::None), data( json{ } ) { assert(false); }
            Tech(TechID id_, TechType techType_, json data_) : id(id_), techType(techType_), data(std::move(data_)) {}

            Tech(TechID id_, TechType techType_, json data_, DependencyType dependencyType_) :
                id(id_), techType(techType_), data(std::move(data_)), dependencyType(dependencyType_) {}

            void addSource   (int techID) { sources.insert(techID); }
            void removeSource(int techID) { sources.erase (techID); }
        
            void addDependent   (int techID) { dependents.insert(techID); }
            void removeDependent(int techID) { dependents.erase (techID); }
        
            bool hasSources() const { return !sources.empty(); }

            void to_json(json& j) const {
                j = json{
                    {ST_ID, id},
                    {ST_DATA, data},
                    {ST_TECH_TYPE, static_cast<int>(techType)},
                    {ST_SOURCES, sources},
                    {ST_DEPENDENTS, dependents},
                    {ST_DEPENDENCY_TYPE, static_cast<int>(dependencyType)}
                };
            }
            
            void from_json(const json& j) {
                id = j.at(ST_ID).get<int>();
                techType = static_cast<TechType>(j.at(ST_TECH_TYPE).get<int>());
                data = j.at(ST_DATA);
                dependencyType = static_cast<DependencyType>(j.at(ST_DEPENDENCY_TYPE).get<int>());
                for (int source    : j.at(ST_SOURCES   )) sources   .insert(source   );
                for (int dependent : j.at(ST_DEPENDENTS)) dependents.insert(dependent);
            }
            
            // static std::unique_ptr<Tech> from_json(const json& j) {
            //     auto tech = std::make_unique<Tech>(
            //         j.at(ST_ID).get<int>(),
            //         static_cast<TechType>(j.at(ST_TECH_TYPE).get<int>()),
            //         j.at(ST_DATA)
            //     );
            //     tech->dependencyType = static_cast<DependencyType>(j.at(ST_DEPENDENCY_TYPE).get<int>());
            //     for (int source    : j.at(ST_SOURCES   )) tech->sources   .insert(source   );
            //     for (int dependent : j.at(ST_DEPENDENTS)) tech->dependents.insert(dependent);
            //     return std::move(tech);
            // }
            
    };

    using UTech = std::unique_ptr<Tech>;
    using Techs = std::vector<UTech>;

}
