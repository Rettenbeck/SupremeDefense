#pragma once

#include <ECS/component_registry.hpp>


namespace SupDef {
    
    DEFINE_COMPONENT_BEGIN(PlayerOwnershipComponent, SC_PLAYER_OWNERSHIP)
        EntityID ownerID;
        EntityIDs accessors;

        PlayerOwnershipComponent(EntityID ownerID_) {
            setInitialOwner(ownerID_);
            addToRegistry();
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

        REFLECT_COMPONENT_BEGINX
            REFLECT_FIELD(ownerID)
            REFLECT_FIELD(accessors)
        REFLECT_COMPONENT_END()
    DEFINE_COMPONENT_END
}
