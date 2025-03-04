#include <EventDispatcher/event.hpp>
#include <ECS/entity.hpp>
#include <Action/action.hpp>

#pragma once


namespace SupDef {
    

    // #### General events #########################################################################
    struct WindowResizeEvent : public Event {
        unsigned width, height;
        WindowResizeEvent(unsigned width_, unsigned height_) : width(width_), height(height_) {}
    };

    struct GameEndEvent : public Event {
        GameEndEvent() {}
    };
    



    // #### General events #########################################################################
    struct StartCommandEvent : public Event {
        CommandID commandID;
        json data;
        StartCommandEvent(const CommandID& command) : commandID(command), data(json{}) {}
        StartCommandEvent(const CommandID& command, const json& jsonData)
            : commandID(command), data(jsonData) {}
    };

    struct UpdateCommandEvent : public Event {
        json data;
        explicit UpdateCommandEvent(const json& jsonData) : data(jsonData) {}
    };
    
    struct ConfirmCommandEvent : public Event {
        bool isConfirmed;
        bool isDirect = false;
        json data;
        explicit ConfirmCommandEvent(bool confirmed, const json& jsonData)
            : isConfirmed(confirmed), data(jsonData) {}
        explicit ConfirmCommandEvent(bool confirmed, const json& jsonData, bool direct)
            : isConfirmed(confirmed), data(jsonData), isDirect(direct) {}
    };

    struct CommandToRenderEvent : public Event {
        CommandID commandID;
        json data;
        Entity* virtualEntity = nullptr;
        explicit CommandToRenderEvent(const CommandID& command, const json& jsonData)
            : commandID(command), data(jsonData) {}
        explicit CommandToRenderEvent(const CommandID& command, const json& jsonData, Entity* entity)
        : commandID(command), data(jsonData), virtualEntity(entity) {}
    };




    // #### General events #########################################################################
    struct UnitSelectedEvent : public Event {
        Entity* entity;
        UnitSelectedEvent(Entity* entity_) : entity(entity_) {}
    };

    


    // #### General events #########################################################################
    struct ActionCreatedEvent : public Event {
        SAction action;
        ActionCreatedEvent(SAction action_) : action(std::move(action_)) {}
    };

    struct EntityDestroyedEvent : public Event {
        EntityID entityID;
        EntityDestroyedEvent(EntityID id) : entityID(id) {}
    };


}
