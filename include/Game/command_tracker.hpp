#pragma once

#include <ECS/include.hpp>


namespace SupDef {

    struct CommandTracker {
        bool ongoing = false;
        EntityID entityID = NO_ENTITY;
        EntityID techID = NO_ENTITY;
        CommandID commandID = NO_COMMAND;
        
        CommandTracker() {}

        void reset() { ongoing = false; }
        void set(EntityID entityID_, EntityID techID_, CommandID commandID_) {
            ongoing = true;
            entityID = entityID_;
            techID = techID_;
            commandID = commandID_;
        }
    };

    using UCommandTracker = std::unique_ptr<CommandTracker>;

}
