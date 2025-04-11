#pragma once

#include <ECS/component_registry.hpp>


namespace SupDef {

    DEFINE_COMPONENT_BEGIN(GiftTechComponent, SCT_GIFT_TECH)
        AssetIDs techsToCreate;
        std::unordered_map<EntityID, EntityIDs> createdTechs;

        GiftTechComponent(AssetIDs techsToCreate) : techsToCreate(techsToCreate) { addToRegistry(); }

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

        bool isTech() const override { return true; }

        REFLECT_COMPONENT_BEGINX
            REFLECT_FIELD(techsToCreate)
            REFLECT_FIELD(createdTechs)
        REFLECT_COMPONENT_END()
    DEFINE_COMPONENT_END

}
