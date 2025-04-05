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
    



    // #### Command events #########################################################################
    struct TriggerCommandEvent : public Event {
        EntityID entityID, techID;
        json data;
        bool cancel;
        TriggerCommandEvent() : entityID(NO_ENTITY), techID(NO_ENTITY), data(json()), cancel(true) {}
        TriggerCommandEvent(const EntityID entityID, const EntityID techID, const json& jsonData)
        : entityID(entityID), techID(techID), data(jsonData), cancel(false) {}
        TriggerCommandEvent(const EntityID entityID, const EntityID techID, const json& jsonData, bool cancel)
        : entityID(entityID), techID(techID), data(jsonData), cancel(cancel) {}
    };

    struct UpdateCommandEvent : public Event {
        json data;
        explicit UpdateCommandEvent(const json& jsonData) : data(jsonData) {}
    };
    
    struct CommandToRenderEvent : public Event {
        CommandID commandID;
        EntityID entityID, techID;
        json data;
        explicit CommandToRenderEvent(const CommandID& command,
            const EntityID entityID, const EntityID techID, const json& jsonData)
            : commandID(command), entityID(entityID), techID(techID), data(jsonData) {}
    };




    // #### Gui events #############################################################################
    struct UnitSelectedEvent : public Event {
        Entity* entity;
        UnitSelectedEvent(Entity* entity_) : entity(entity_) {}
    };

    


    // #### Action events ##########################################################################
    struct ActionCreatedEvent : public Event {
        SAction action;
        ActionCreatedEvent(SAction action_) : action(std::move(action_)) {}
    };

    struct EntityDestroyedEvent : public Event {
        EntityID entityID;
        EntityDestroyedEvent(EntityID id) : entityID(id) {}
    };

    


    // #### Network events #########################################################################
    struct GameBlockedEvent : public Event {
        bool blocked;
        GameBlockedEvent(bool blocked = true) : blocked(blocked) {}
    };



}
