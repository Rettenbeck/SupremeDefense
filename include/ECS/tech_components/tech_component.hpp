#include <ECS/component_registry.hpp>

#pragma once


namespace SupDef {

    struct TechComponent : public Component {
        EntityIDs assignees;

        TechComponent() { addToRegistry(); }

        void addToRegistry() {
            ComponentRegistry::registerComponent(getTypeName(), []()
                { return std::make_unique<TechComponent>(); });
        }

        void addAssignee(EntityID entityID) {
            if (std::find(assignees.begin(), assignees.end(), entityID) == assignees.end()) {
                assignees.push_back(entityID);
            }
        }

        void removeAssignee(EntityID entityID) {
            assignees.erase(std::remove(assignees.begin(), assignees.end(), entityID), assignees.end());
        }

        void to_json(json& j) const override {
            j = json{{S_ASSIGNEES, assignees}};
        }

        void from_json(const json& j) override {
            j.at(S_ASSIGNEES).get_to(assignees);
        }

        std::string getTypeName() const override {
            return SCT_TECH;
        }

    };

}
