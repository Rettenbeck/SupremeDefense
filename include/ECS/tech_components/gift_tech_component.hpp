#include <ECS/component_registry.hpp>

#pragma once


namespace SupDef {

    struct GiftTechComponent : public Component {
        AssetIDs techsToCreate;
        std::unordered_map<EntityID, EntityIDs> createdTechs;

        GiftTechComponent() { addToRegistry(); }
        GiftTechComponent(AssetIDs techsToCreate) : techsToCreate(techsToCreate) { addToRegistry(); }

        void addToRegistry() {
            ComponentRegistry::registerComponent(getTypeName(), []()
                { return std::make_unique<GiftTechComponent>(); });
        }

        void addTech(AssetID assetID) {
            push_back_unique(techsToCreate, assetID);
        }

        void removeTech(AssetID assetID) {
            remove_all(techsToCreate, assetID);
        }

        void addCreatedTech(EntityID assignee, EntityID techID) {
            auto& vec = createdTechs[assignee];
            push_back_unique(vec, techID);
        }

        void removeCreatedTech(EntityID assignee, EntityID techID) {
            auto& vec = createdTechs[assignee];
            remove_all(vec, techID);
            if (vec.empty()) createdTechs.erase(assignee);
        }

        void to_json(json& j) const override {
            j[S_TECHS_TO_CREATE] = techsToCreate;
            j[S_CREATED_TECHS  ] = createdTechs;
        }

        void from_json(const json& j) override {
            j.at(S_TECHS_TO_CREATE).get_to(techsToCreate);
            j.at(S_CREATED_TECHS  ).get_to(createdTechs);
        }

        std::string getTypeName() const override {
            return SCT_GIFT_TECH;
        }

        bool isTech() const override { return true; }

    };

}
