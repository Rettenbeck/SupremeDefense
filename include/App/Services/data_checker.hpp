#pragma once

#include <ECS/asset_manager.hpp>
#include <ECS/entity_manager.hpp>
#include <Game/collision_system.hpp>
#include <Game/replay.hpp>


namespace SupDef {

    class DataChecker {
        private:
            // json j_replay, j_assets, j_entities, j_collisions;
            // json j_complete;
            // GameStarterStatus status = GameStarterStatus::NoGame;
            // std::string message = "";

        public:
            DataChecker() { }

    };

    using UDataChecker = std::unique_ptr<DataChecker>;

}
