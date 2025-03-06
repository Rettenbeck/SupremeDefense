#include <ECS/include.hpp>
#include <App/Log/logger.hpp>

#pragma once


namespace SupDef {

    using _EntPosColImm  = std::tuple<Entity*, PositionComponent*, CollisionComponent*, ImmovableComponent*>;
    using _EntPosColImms = std::vector<_EntPosColImm>;

    class TilesChecker {
        private:
            Ints getAffectedTilesForShape(float cx, float cy, CollisionComponent* collisionComponent, TilesComponent* tilesComponent) {
                std::vector<int> vec;
                auto rect = std::make_unique<CollisionComponent>();
                auto shape = std::make_unique<RectangleShape>(0, 0, tilesComponent->tileSize, tilesComponent->tileSize);
                auto shapePtr = shape.get();
                rect->addShape(std::move(shape));

                int x = -1, y = 0, index = -1;
                for(auto& t : tilesComponent->tiles) {
                    index++;
                    if(++x >= tilesComponent->tilesX) {
                        x = 0; y++;
                    }
                    shapePtr->offsetX = x * tilesComponent->tileSize;
                    shapePtr->offsetY = y * tilesComponent->tileSize;
                    if(collisionComponent->collidesWith(rect.get(), cx, cy, 0, 0)) {
                        vec.push_back(index);
                    }
                }
                return vec;
            }

            inline void setTileByShape(ImmovableComponent* immovableComponent, TilesComponent* tilesComponent, int tileIndex) {
                assert(tileIndex >= 0 && tileIndex < tilesComponent->tiles.size());
                auto& t = tilesComponent->tiles[tileIndex];
                if(immovableComponent->marksTilesOccupied  ) t->isOccupied   = true;
                if(immovableComponent->marksTilesImpassable) t->isImpassable = true;
            }

            void setTilesByShape(ImmovableComponent* immovableComponent, TilesComponent* tilesComponent, Ints& vec) {
                for(auto& e : vec) {
                    setTileByShape(immovableComponent, tilesComponent, e);
                }
            }

            EntityIDs getAffectedShapesForTiles(Ints tileIndeces, _EntPosColImms& comps, TilesComponent* tilesComponent) {
                EntityIDs affected;
                auto rect = std::make_unique<CollisionComponent>();
                auto shape = std::make_unique<RectangleShape>(0, 0, tilesComponent->tileSize, tilesComponent->tileSize);
                auto shapePtr = shape.get();
                rect->addShape(std::move(shape));

                for(auto& comp : comps) {
                    auto entity = std::get<0>(comp);
                    auto pos = std::get<1>(comp);
                    auto col = std::get<2>(comp);
                    assert(pos); assert(col);
                    for(auto t : tileIndeces) {
                        auto coord = tilesComponent->getPosFromIndex(t);
                        shapePtr->offsetX = coord.x;
                        shapePtr->offsetY = coord.y;
                        if(col->collidesWith(rect.get(), pos->x, pos->y, 0, 0)) {
                            affected.push_back(entity->id);
                            break;
                        }
                    }
                }
                return affected;
            }

        public:
            TilesChecker() {}
            
            void setTilesAfterPlacing(_EntPosColImm comp, TilesComponent* tilesComponent) {
                auto pos = std::get<1>(comp);
                auto col = std::get<2>(comp);
                auto imm = std::get<3>(comp);
                assert(pos); assert(col); assert(imm);

                imm->placedDown = true;
                if(!imm->marksTilesOccupied && !imm->marksTilesImpassable) return;
                auto tiles = getAffectedTilesForShape(pos->x, pos->y, col, tilesComponent);
                setTilesByShape(imm, tilesComponent, tiles);
            }

            void setTilesAfterPlacing(Entity* entity, TilesComponent* tilesComponent) {
                auto pos = entity->getComponent<PositionComponent>();
                auto col = entity->getComponent<CollisionComponent>();
                auto imm = entity->getComponent<ImmovableComponent>();
                return setTilesAfterPlacing(_EntPosColImm(entity, pos, col, imm), tilesComponent);
            }

            void resetTilesAfterRemoving(_EntPosColImm comp, TilesComponent* tilesComponent, EntityManager* entityManager) {
                auto entity = std::get<0>(comp);
                auto pos = std::get<1>(comp);
                auto col = std::get<2>(comp);
                auto imm = std::get<3>(comp);
                assert(pos); assert(col); assert(imm);
                imm->placedDown = false;
                if(!imm->marksTilesOccupied && !imm->marksTilesImpassable) return;
                
                auto tiles = getAffectedTilesForShape(pos->x, pos->y, col, tilesComponent);
                tilesComponent->setTilesToDefault(tiles);

                auto all_entities = entityManager->getEntitiesWithComponents<PositionComponent, CollisionComponent, ImmovableComponent>();
                auto entities = getAffectedShapesForTiles(tiles, all_entities, tilesComponent);
                for(auto entityID : entities) {
                    if(entityID == entity->id) continue;
                    auto tmp = entityManager->getEntity(entityID);
                    assert(tmp);
                    imm = tmp->getComponent<ImmovableComponent>();
                    if(!imm->placedDown) continue;
                    setTilesAfterPlacing(tmp, tilesComponent);
                }
            }

    };

    using UTilesChecker = std::unique_ptr<TilesChecker>;

}
