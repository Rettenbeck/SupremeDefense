#include <ECS/component_registry.hpp>

#pragma once


namespace SupDef {

    struct PlayerOwnershipComponent : public Component {
        EntityID ownerID;
        EntityIDs accessors;

        PlayerOwnershipComponent(EntityID ownerID_) {
            setInitialOwner(ownerID_);
            addToRegistry();
        }

        PlayerOwnershipComponent() { addToRegistry(); }

        void addToRegistry() {
            ComponentRegistry::registerComponent(getTypeName(), []()
                { return std::make_unique<PlayerOwnershipComponent>(); });
        }

        void setInitialOwner(EntityID ownerID_) {
            accessors.clear();
            ownerID = ownerID_;
            accessors.push_back(ownerID_);
        }

        void addAccessor(EntityID entityID) {
            if (std::find(accessors.begin(), accessors.end(), entityID) == accessors.end()) {
                accessors.push_back(entityID);
            }
        }

        void removeAccessor(EntityID entityID) {
            accessors.erase(std::remove(accessors.begin(), accessors.end(), entityID), accessors.end());
        }

        void to_json(json& j) const override {
            generic_to_json(j, this);
        }

        void from_json(const json& j) override {
            generic_from_json(j, this);
        }

        REFLECT_COMPONENT_BEGIN(PlayerOwnershipComponent)
            REFLECT_FIELD(ownerID)
            REFLECT_FIELD(accessors)
        REFLECT_COMPONENT_END()

        // void to_json(json& j) const override {
        //     j = json{{S_OWNER, ownerID}, {S_ACCESSORS, accessors}};
        // }

        // void from_json(const json& j) override {
        //     j.at(S_OWNER).get_to(ownerID);
        //     j.at(S_ACCESSORS).get_to(accessors);
        // }

        std::string getTypeName() const override {
            return SC_PLAYER_OWNERSHIP;
        }

    };

}
