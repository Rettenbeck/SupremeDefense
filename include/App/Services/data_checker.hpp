#pragma once

#include <ECS/asset_manager.hpp>
#include <ECS/entity_manager.hpp>
#include <Game/collision_system.hpp>
#include <Game/replay.hpp>


namespace SupDef {

    class DataChecker {
        private:
            UAssetManager assetManager;
            UEntityManager entityManager;
            UCollisionSystem collisionSystem;
            UReplay replay;

        public:
            DataChecker() { }

            void clear() {
                assetManager.reset();
                entityManager.reset();
                collisionSystem.reset();
                replay.reset();
            }

            bool checkAssetManager(json& j) {
                try {
                    assetManager->from_json(j);
                    return true;
                } catch(const std::exception& e) {
                    LOG_ERROR(e.what());
                    return false;
                }
            }

            bool checkEntityManager(json& j) {
                try {
                    entityManager->from_json(j);
                    return true;
                } catch(const std::exception& e) {
                    LOG_ERROR(e.what());
                    return false;
                }
            }

            bool checkCollisionSystem(json& j) {
                try {
                    collisionSystem->from_json(j);
                    return true;
                } catch(const std::exception& e) {
                    LOG_ERROR(e.what());
                    return false;
                }
            }

            bool checkReplay(json& j) {
                try {
                    replay->from_json(j);
                    return true;
                } catch(const std::exception& e) {
                    LOG_ERROR(e.what());
                    return false;
                }
            }

    };

    using UDataChecker = std::unique_ptr<DataChecker>;

}
