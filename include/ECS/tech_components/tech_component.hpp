#include <ECS/component_registry.hpp>

#pragma once


namespace SupDef {

    struct TechComponent : public Component {
        EntityIDs assignees, gained, kept, lost;

        bool applyToAll = false;
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
    
        TechComponent() { addToRegistry(); }
        TechComponent(TechComponent* other) {
            copyFrom(other);
            addToRegistry();
        }

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

        void addRequiredComponentFromString(std::string name) {
            if (std::find(requiredComponents.begin(), requiredComponents.end(), name) == requiredComponents.end()) {
                requiredComponents.push_back(name);
            }
        }

        template <typename... ComponentTypes>
        void addRequiredComponents() {
            (..., ([] {
                using T = ComponentTypes;
                T instance;
                addRequiredComponentFromString(instance.getTypeName());
            }()));
        }

        void copyFrom(TechComponent* other) {
            if (!other) return;
            applyToAll              = other->applyToAll;
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

        void to_json(json& j) const override {
            j[S_GAINED               ] = gained;
            j[S_KEPT                 ] = kept;
            j[S_LOST                 ] = lost;
            j[S_APPLY_TO_ALL         ] = applyToAll;
            j[S_APPLY_TO_PARENT      ] = applyToParent;
            j[S_APPLY_TO_CHILDREN    ] = applyToChildren;
            j[S_APPLY_TO_INFLUENCE   ] = applyToWithinInfluence;
            j[S_AFFECTS_ALL          ] = affectsAll;
            j[S_AFFECTS_SAME_PLAYER  ] = affectsSamePlayer;
            j[S_AFFECTS_SAME_TEAM    ] = affectsSameTeam;
            j[S_AFFECTS_ENEMY        ] = affectsEnemy;
            j[S_FILTER_BY_COMPONENTS ] = filterByComponents;
            j[S_REQUIRED_COMPONENTS  ] = requiredComponents;
            j[S_WHITELIST            ] = whitelist;
            j[S_BLACKLIST            ] = blacklist;
        }

        void from_json(const json& j) override {
            j.at(S_GAINED               ).get_to(gained                );
            j.at(S_KEPT                 ).get_to(kept                  );
            j.at(S_LOST                 ).get_to(lost                  );
            j.at(S_APPLY_TO_ALL         ).get_to(applyToAll            );
            j.at(S_APPLY_TO_PARENT      ).get_to(applyToParent         );
            j.at(S_APPLY_TO_CHILDREN    ).get_to(applyToChildren       );
            j.at(S_APPLY_TO_INFLUENCE   ).get_to(applyToWithinInfluence);
            j.at(S_AFFECTS_ALL          ).get_to(affectsAll            );
            j.at(S_AFFECTS_SAME_PLAYER  ).get_to(affectsSamePlayer     );
            j.at(S_AFFECTS_SAME_TEAM    ).get_to(affectsSameTeam       );
            j.at(S_AFFECTS_ENEMY        ).get_to(affectsEnemy          );
            j.at(S_FILTER_BY_COMPONENTS ).get_to(filterByComponents    );
            j.at(S_REQUIRED_COMPONENTS  ).get_to(requiredComponents    );
            j.at(S_WHITELIST            ).get_to(whitelist             );
            j.at(S_BLACKLIST            ).get_to(blacklist             );
        }

        std::string getTypeName() const override {
            return SCT_TECH;
        }

        bool isTech() const override { return true; }

    };

}
