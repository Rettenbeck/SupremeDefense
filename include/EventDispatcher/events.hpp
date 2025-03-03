#include <EventDispatcher/event.hpp>
#include <ECS/entity.hpp>
#include <Action/action.hpp>

#pragma once


namespace SupDef {
    

    // #### General events #####################################
    struct WindowResizeEvent : public Event {
        unsigned width, height;
        WindowResizeEvent(unsigned width_, unsigned height_) : width(width_), height(height_) {}
    };

    struct GameEndEvent : public Event {
        GameEndEvent() {}
    };


    // #### Command events #####################################
    struct StartCommandEvent : public Event {
        CommandID commandID;
        StartCommandEvent(const CommandID& command) : commandID(command) {}
    };

    struct StartCommandReceivedEvent : public Event {
        //CommandID commandID;
        bool success;
        StartCommandReceivedEvent(bool isSuccess) : success(isSuccess) {}
        // StartCommandReceivedEvent(const CommandID& command, bool isSuccess)
        //     : commandID(command), success(isSuccess) {}
    };

    struct UpdateCommandEvent : public Event {
        json data;
        explicit UpdateCommandEvent(const json& jsonData) : data(jsonData) {}
    };
    
    struct UpdateCommandReceivedEvent : public Event {
        json data;
        explicit UpdateCommandReceivedEvent(const json& jsonData) : data(jsonData) {}
    };

    struct ConfirmCommandEvent : public Event {
        bool isConfirmed;
        json data;
        explicit ConfirmCommandEvent(bool confirmed, const json& jsonData)
            : isConfirmed(confirmed), data(jsonData) {}
    };

    struct ConfirmCommandReceivedEvent : public Event {
        bool success;
        json data;
        explicit ConfirmCommandReceivedEvent(bool isSuccess, const json& jsonData)
            : success(isSuccess), data(jsonData) {}
    };
    

    // #### Specific command events ############################
    struct UnitSelectedEvent : public Event {
        Entity* entity;
        UnitSelectedEvent(Entity* entity_) : entity(entity_) {}
    };


    // #### Other events #######################################
    struct ActionCreatedEvent : public Event {
        SAction action;
        ActionCreatedEvent(SAction action_) : action(std::move(action_)) {}
    };

    struct EntityDestroyedEvent : public Event {
        EntityID entityID;
        EntityDestroyedEvent(EntityID id) : entityID(id) {}
    };


}
