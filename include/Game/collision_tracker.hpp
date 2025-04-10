#pragma once

#include <Game/collision_system.hpp>


namespace SupDef {

    struct CollisionInfo {
        EntityID entityA;
        EntityID entityB;
        int frameCount;       // 1 = new collision, >1 = ongoing, <1 = just ended
        CollisionGroup collisionGroup;
    
        CollisionInfo(EntityID a, EntityID b, CollisionGroup group)
            : entityA(a), entityB(b), frameCount(1), collisionGroup(std::move(group)) {}
    
        bool isNew() { return frameCount == 1; }
        bool hasEnded() { return frameCount < 1; }

        bool operator==(const CollisionInfo& other) const {
            return (entityA == other.entityA && entityB == other.entityB) ||
                   (entityA == other.entityB && entityB == other.entityA);  // Order doesn't matter
        }
        
        bool equals(const EntityID a, const EntityID b) {
            return (entityA == a && entityB == b) ||
                   (entityA == b && entityB == a);  // Order doesn't matter
        }
    };

    using UCollisionInfo = std::unique_ptr<CollisionInfo>;
    using CollisionInfos = std::vector<UCollisionInfo>;
    using PCollisionInfos = std::vector<CollisionInfo*>;

    class CollisionTracker {
        public:
            void updateCollisions(const CollisionPairs& detectedCollisions, CollisionGroup collisionGroup) {
                // Step 1: Mark all existing collisions of this group as unchecked (-X)
                for (auto& col : collisionList) {
                    if (col->collisionGroup == collisionGroup) {
                        assert(col->frameCount >= 0);
                        col->frameCount *= -1; // Mark as unchecked
                    }
                }

                // Step 2: Process detected collisions for this group
                for (const auto& [a, b] : detectedCollisions) {
                    auto it = std::find_if(collisionList.begin(), collisionList.end(),
                                        [&](const UCollisionInfo& col) {
                                            return col->collisionGroup == collisionGroup && col->equals(a->id, b->id);
                                        });
                    
                    auto col = (*it).get();
                    if (it != collisionList.end()) {
                        // Collision already existed → mark as checked (make frameCount positive) and increment it
                        if (col->frameCount <= 0) {
                            col->frameCount *= -1; // Convert negative to positive
                        }
                        col->frameCount++; // Increase counter
                    } else {
                        // New collision → Add it with frameCount = 1
                        collisionList.emplace_back(std::make_unique<CollisionInfo>(a->id, b->id, collisionGroup));
                    }
                }
            }

            CollisionInfos& getCollisionList() { return collisionList; }

            CollisionInfo* retrieve(EntityID entityA, EntityID entityB) {
                for (auto& collision : collisionList) {
                    if ((collision->entityA == entityA && collision->entityB == entityB) ||
                        (collision->entityA == entityB && collision->entityB == entityA)) {
                            return collision.get();
                        }
                }
                return nullptr;
            }

            PCollisionInfos retrieve(EntityID entity) {
                PCollisionInfos result;
                for (auto& collision : collisionList) {
                    if (collision->entityA == entity || collision->entityB == entity) {
                        result.push_back(collision.get());
                    }
                }
                return result;
            }

            void to_json(json& j) const {
                for (const auto& col : collisionList) {
                    j.push_back({
                        {SG_ENTITY_A        , col->entityA       },
                        {SG_ENTITY_B        , col->entityB       },
                        {SG_FRAME_COUNT     , col->frameCount    },
                        {SG_COLLISION_GROUP , col->collisionGroup}
                    });
                }
            }
        
            void from_json(const json& j) {
                collisionList.clear();
                for (const auto& item : j) {
                    auto ci = std::make_unique<CollisionInfo>(item[SG_ENTITY_A], item[SG_ENTITY_B], item[SG_COLLISION_GROUP]);
                    ci->frameCount = item[SG_FRAME_COUNT];
                    collisionList.emplace_back(std::move(ci));
                }
            }
        
        private:
            CollisionInfos collisionList;

    };

    using UCollisionTracker = std::unique_ptr<CollisionTracker>;

}
