#include <ECS/component_registry.hpp>

#pragma once


namespace SupDef {

    struct GiftTechComponent : public Component {
        AssetIDs techsToApply;

        GiftTechComponent() { addToRegistry(); }
        GiftTechComponent(AssetIDs techsToApply) : techsToApply(techsToApply) { addToRegistry(); }

        void addToRegistry() {
            ComponentRegistry::registerComponent(getTypeName(), []()
                { return std::make_unique<GiftTechComponent>(); });
        }

        void addTech(AssetID assetID) {
            if (std::find(techsToApply.begin(), techsToApply.end(), assetID) == techsToApply.end()) {
                techsToApply.push_back(assetID);
            }
        }

        void removeTech(AssetID assetID) {
            techsToApply.erase(std::remove(techsToApply.begin(), techsToApply.end(), assetID), techsToApply.end());
        }

        void to_json(json& j) const override {
            j[S_TECHS_TO_APPLY] = techsToApply;
        }

        void from_json(const json& j) override {
            j.at(S_TECHS_TO_APPLY).get_to(techsToApply);
        }

        std::string getTypeName() const override {
            return SCT_GIFT_TECH;
        }

        bool isTech() const override { return true; }

    };

}
