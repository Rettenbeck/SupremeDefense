#include <ECS/include.hpp>
#include <Game/constants.hpp>
#include <App/Log/logger.hpp>

#pragma once


namespace SupDef {
    
    struct Collider {
        Entity* entity;
        PositionComponent*  pos;
        CollisionComponent* col;
        bool group;
        Collider(Entity* entity_, PositionComponent* pos_, CollisionComponent* col_, bool group_)
            : entity(entity_), pos(pos_), col(col_), group(group_) {}
    };
    
    struct CollisionPair {
        Entity* a;
        Entity* b;
        CollisionPair(Entity* a_, Entity* b_) : a(a_), b(b_) {}
    };
    
    using Colliders = std::vector<Collider*>;
    using CollisionPairs = std::vector<CollisionPair>;

    class CollisionSystem {
        private:
            using GridMap = std::unordered_map<int, Colliders>;

            int tileSize;
            GridMap gridA, gridB;
            Colliders largeObjectsA, largeObjectsB;
            bool influenceMode = false;

            int collisionChecksExpected = 0, collisionChecksActual = 0, collisionChecksNarrow = 0, collisionsTotal = 0;
            
            void checkCollisions(const Colliders& listA, const Colliders& listB, CollisionPairs& collisions, bool sameGroup) {
                for (size_t i = 0; i < listA.size(); ++i) {
                    for (size_t j = 0; j < listB.size(); ++j) {
                        auto a = listA[i]; auto b = listB[j];
                        collisionChecksActual++;

                        if (a->entity->id == b->entity->id) continue;
                        if (sameGroup && a->entity->id > b->entity->id) continue;
                        collisionChecksNarrow++;

                        bool collide;
                        if (influenceMode) {
                            collide = a->col->withinInfluence(b->col, a->pos->x, a->pos->y, b->pos->x, b->pos->y);
                        } else {
                            collide = a->col->collidesWith(b->col, a->pos->x, a->pos->y, b->pos->x, b->pos->y);
                        }

                        if (collide) {
                            collisions.push_back(CollisionPair(a->entity, b->entity));
                            collisionsTotal++;
                        }
                    }
                }
            }
        
            void checkCollisionsLargeObjectsAgainstGrid(const Colliders& largeObjects, const GridMap& grid, CollisionPairs& collisions, bool sameGroup) {
                for (auto* largeObj : largeObjects) {
                    for (const auto& [tilePos, tileObjects] : grid) {
                        checkCollisions({largeObj}, tileObjects, collisions, sameGroup);
                    }
                }
            }

        public:
            CollisionSystem(int tileSize_ = 64) : tileSize(tileSize_) {}
            
            CollisionPairs findCollisions(const Colliders& objects, int width, int height, bool groupMode = false) {
                CollisionPairs collisions;

                int nGroup1 = 0, nGroup2 = 0;
                collisionChecksActual = 0;
                collisionChecksNarrow = 0;
                collisionsTotal = 0;
                
                int tilesX = width  / tileSize;
                int tilesY = height / tileSize;
                if(width  % tileSize > 0) tilesX++;
                if(height % tileSize > 0) tilesY++;

                gridA.clear();
                gridB.clear();
                largeObjectsA.clear();
                largeObjectsB.clear();
                
                auto getKey = [tilesX](int x, int y) { return tilesX * y + x; };
                auto getX   = [tilesX](int key)      { return key % tilesX; };
                auto getY   = [tilesX](int key)      { return key / tilesX; };
                auto oob    = [tilesX, tilesY](int x, int y) { return !(x >= 0 && y >= 0 && x < tilesX & y < tilesY); };

                auto addToGrid = [&](Collider* objToAdd, int keyToAdd) {
                    if (objToAdd->group) {
                        gridB[keyToAdd].push_back(objToAdd);
                        nGroup2++;
                    } else {
                        gridA[keyToAdd].push_back(objToAdd);
                        nGroup1++;
                    }
                };
                
                auto addToLarge = [&](Collider* objToAdd) {
                    if (objToAdd->group) {
                        largeObjectsB.push_back(objToAdd);
                        nGroup2++;
                    } else {
                        largeObjectsA.push_back(objToAdd);
                        nGroup1++;
                    }
                };
                
                for (auto* obj : objects) {
                    if (obj->col->boundingBox.w > tileSize || obj->col->boundingBox.h > tileSize) {
                        addToLarge(obj);
                    } else {
                        assert(obj->col->boundingBox.isDefined);
                        int tileX = static_cast<int>((obj->pos->x + obj->col->boundingBox.x) / tileSize);
                        int tileY = static_cast<int>((obj->pos->y + obj->col->boundingBox.y) / tileSize);

                        if (oob(tileX, tileY)) {
                            addToLarge(obj);
                        } else {
                            addToGrid(obj, getKey(tileX, tileY));
                        }
                    }
                }
                
                GridMap* grid = groupMode ? &gridB : &gridA;
                for (const auto& [tilePos, tileObjects] : gridA) {
                    int tx = getX(tilePos);
                    int ty = getY(tilePos);
                    
                    for (int dx = -1; dx <= 1; ++dx) {
                        for (int dy = -1; dy <= 1; ++dy) {
                            int nx = tx + dx, ny = ty + dy;
                            if (oob(nx, ny)) continue;

                            auto neighborTile = grid->find(getKey(nx, ny));
                            if (neighborTile != grid->end()) {
                                checkCollisions(tileObjects, neighborTile->second, collisions, !groupMode);
                            }
                        }
                    }
                }
                
                if (groupMode) {
                    checkCollisions(largeObjectsA, largeObjectsB, collisions, false);
                    checkCollisionsLargeObjectsAgainstGrid(largeObjectsA, gridB, collisions, false);
                    checkCollisionsLargeObjectsAgainstGrid(largeObjectsB, gridA, collisions, false);
                } else {
                    checkCollisions(largeObjectsA, largeObjectsA, collisions, true);
                    checkCollisionsLargeObjectsAgainstGrid(largeObjectsA, gridA, collisions, false);
                }
                
                if (groupMode) {
                    collisionChecksExpected = nGroup1 * nGroup2;
                } else {
                    collisionChecksExpected = nGroup1 * nGroup1;
                }
                
                return collisions;
            }
        
            int getTileSize() { return tileSize; }
            void setInfluenceMode(bool influenceMode_) { influenceMode = influenceMode_; }

            using ColStats = std::tuple<int, int, int, int>;
            ColStats getCollisionStats() {
                ColStats stats(collisionChecksExpected, collisionChecksActual, collisionChecksNarrow, collisionsTotal);
                return stats;
            }

    };
    
    using UCollisionSystem = std::unique_ptr<CollisionSystem>;

}
