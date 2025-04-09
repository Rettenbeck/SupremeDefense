#include <EventDispatcher/event.hpp>
#include <ECS/entity.hpp>
#include <Action/action.hpp>
#include <Action/action_queue.hpp>

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
    struct SetPlayerNameEvent : public Event {
        std::string name;
        SetPlayerNameEvent(std::string name) : name(name) {}
    };

    struct StartNetworkGameAsServerEvent : public Event {
        unsigned short port;
        StartNetworkGameAsServerEvent() : port(9000) {}
        StartNetworkGameAsServerEvent(unsigned short port) : port(port) {}
    };

    struct StartNetworkGameAsClientEvent : public Event {
        unsigned short port;
        std::string ip;
        StartNetworkGameAsClientEvent() : ip("127.0.0.1"), port(9000) {}
        StartNetworkGameAsClientEvent(std::string ip, unsigned short port) : ip(ip), port(port) {}
    };

    struct CompleteServerEvent : public Event {
        CompleteServerEvent() {}
    };

    struct StartGameAsServerStatusEvent : public Event {
        bool success;
        StartGameAsServerStatusEvent(bool success) : success(success) {}
    };

    struct StartGameAsClientStatusEvent : public Event {
        bool success;
        StartGameAsClientStatusEvent(bool success) : success(success) {}
    };

    struct GameBlockedByNetworkEvent : public Event {
        bool blocked;
        GameBlockedByNetworkEvent(bool blocked = true) : blocked(blocked) {}
    };

    struct GameHasUpdatedEvent : public Event {
        EntityID thisPlayer;
        long frameCount;
        GameHasUpdatedEvent(EntityID thisPlayer, long frameCount)
        : thisPlayer(thisPlayer), frameCount(frameCount) {}
    };

    struct ReceivedActionsFromServerEvent : public Event {
        ActionQueue* actionQueue = nullptr;
        ReceivedActionsFromServerEvent(ActionQueue* actionQueue) : actionQueue(actionQueue) {}
    };

    struct SendPlayerListEvent : public Event {
        EntityIDs playerList;
        SendPlayerListEvent(EntityIDs& playerList) : playerList(playerList) {}
    };

    struct StopNetworkGameEvent : public Event {
        StopNetworkGameEvent() {}
    };


    
}
