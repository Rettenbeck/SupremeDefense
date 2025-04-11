#pragma once

#include <ECS/component_registry.hpp>


namespace SupDef {

    DEFINE_COMPONENT_BEGIN(TechComponent, SCT_TECH)
        EntityIDs assignees, gained, kept, lost;
        EntityID createdBy = NO_ENTITY;

        bool applyToAll = false;
        bool applyToOwner = false;
        bool applyToParent = false;
        bool applyToChildren = false;
        bool applyToWithinInfluence = false;

        bool affectsAll = false;
        bool affectsSamePlayer = false;
        bool affectsSameTeam = false;
        bool affectsEnemy = false;

        bool filterByComponents = false;
        std::vector<std::string> requiredComponents;
        std::unordered_set<EntityID> whitelist;
        std::unordered_set<EntityID> blacklist;
    
        TechComponent(TechComponent* other) {
            copyFrom(other);
            addToRegistry();
        }

        void addAssignee(EntityID entityID) {
            push_back_unique(assignees, entityID);
        }

        void removeAssignee(EntityID entityID) {
            remove_all(assignees, entityID);
        }

        void addRequiredComponentFromString(std::string name) {
            push_back_unique(requiredComponents, name);
        }

        template <typename ComponentType>
        void addRequiredComponent() {
            ComponentType instance;
            addRequiredComponentFromString(instance.getTypeName());
        }

        template <typename... ComponentTypes>
        void addRequiredComponents() {
            (..., addRequiredComponent<ComponentTypes>());
        }

        void copyFrom(TechComponent* other) {
            if (!other) return;
            applyToAll              = other->applyToAll;
            applyToOwner            = other->applyToOwner;
            applyToParent           = other->applyToParent;
            applyToChildren         = other->applyToChildren;
            applyToWithinInfluence  = other->applyToWithinInfluence;
            affectsAll              = other->affectsAll;
            affectsSamePlayer       = other->affectsSamePlayer;
            affectsSameTeam         = other->affectsSameTeam;
            affectsEnemy            = other->affectsEnemy;
            filterByComponents      = other->filterByComponents;
            requiredComponents      = other->requiredComponents;
        }

        bool isTech() const override { return true; }

        REFLECT_COMPONENT_BEGINX
            REFLECT_FIELD(gained)
            REFLECT_FIELD(kept)
            REFLECT_FIELD(lost)
            REFLECT_FIELD(createdBy)
            REFLECT_FIELD(applyToAll)
            REFLECT_FIELD(applyToOwner)
            REFLECT_FIELD(applyToParent)
            REFLECT_FIELD(applyToChildren)
            REFLECT_FIELD(applyToWithinInfluence)
            REFLECT_FIELD(affectsAll)
            REFLECT_FIELD(affectsSamePlayer)
            REFLECT_FIELD(affectsSameTeam)
            REFLECT_FIELD(affectsEnemy)
            REFLECT_FIELD(filterByComponents)
            REFLECT_FIELD(requiredComponents)
            REFLECT_FIELD(whitelist)
            REFLECT_FIELD(blacklist)
        REFLECT_COMPONENT_END()
    DEFINE_COMPONENT_END

}
