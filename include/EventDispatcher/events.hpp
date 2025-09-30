#pragma once

#include <EventDispatcher/event.hpp>
#include <ECS/entity.hpp>
#include <Action/action.hpp>
#include <Action/action_queue.hpp>


namespace SupDef {
    

    // #### General events #########################################################################

    // Is dispatched by the renderer when the window is resized. Useful for the GUI to know.
    struct WindowResizeEvent : public Event {
        unsigned width, height;
        WindowResizeEvent(unsigned width_, unsigned height_) : width(width_), height(height_) {}
    };

    // Is dispatched by the render layer and subscribed to by the App class. Ends the game when
    //   called.
    struct GameEndEvent : public Event {
        GameEndEvent() {}
    };

    // Is dispatched by the render layer and subscribed to by the App class. Starts a test game
    //   with hard-coded parameters.
    struct StartTestGameEvent : public Event {
        StartTestGameEvent() {}
    };

    // Is not dispatched currently; subscribed to by the App class. Makes sure to reassign objects
    //   within the App class once objects have changed.
    struct UpdateAppEvent : public Event {
        UpdateAppEvent() {}
    };
    



    // #### Command events #########################################################################

    // Is dispatched in the render class and subscribed to by the game class. Tells the game that
    //   the player wants to start/execute a command.
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

    // Is dispatched in the render class and subscribed to by the game class. Updates a command
    //   that is already in process.
    struct UpdateCommandEvent : public Event {
        json data;
        explicit UpdateCommandEvent(const json& jsonData) : data(jsonData) {}
    };
    
    // Is dispatched in the game class and subscribed to by the Game class. Forwards in-game data
    //   to the renderer.
    struct CommandToRenderEvent : public Event {
        CommandID commandID;
        EntityID entityID, techID;
        json data;
        explicit CommandToRenderEvent(const CommandID& command,
            const EntityID entityID, const EntityID techID, const json& jsonData)
            : commandID(command), entityID(entityID), techID(techID), data(jsonData) {}
    };




    // #### Gui events #############################################################################

    // Is dispatched by a page object and subscribed to by the page manager. Starts a new page.
    struct GotoPageEvent : public Event {
        int pageId;
        GotoPageEvent(int pageId_) { pageId = pageId_; }
    };

    // Is dispatched by a page object and subscribed to by the page manager. Starts a new page.
    struct PushPageEvent : public Event {
        int pageId;
        PushPageEvent(int pageId_) { pageId = pageId_; }
    };

    // Is dispatched by a page object and subscribed to by the page manager. Closes the top page.
    struct ClosePageEvent : public Event {
        ClosePageEvent() { }
    };




    // #### Gui menu buttons #######################################################################

    struct MenuButtonOpenServerEvent : public Event { MenuButtonOpenServerEvent() { } };
    struct MenuButtonJoinServerEvent : public Event { MenuButtonJoinServerEvent() { } };

    


    // #### Game Gui events ########################################################################
    
    // Is dispatched by the renderer and subscribed to by the gui manager. Contains an event that
    //   is intended for the game logic. This way the gui still has a change about reacting to that
    //   interaction.
    struct GameInteractionEvent : public Event {
        std::function<void()> call;
        GameInteractionEvent() {}
        GameInteractionEvent(std::function<void()> call_) : call(call_) {}
    };

    #define DISPATCH_GIE(DISPATCHER, EVENT, ...) \
        DISPATCHER->dispatch<GameInteractionEvent>( \
            [&]() { DISPATCHER->dispatch<EVENT>(__VA_ARGS__); } \
        );

    // Is dispatched by the renderer and subscribed to by the gui manager. Conveys a movement
    //   command.
    struct GameInteractionMovementEvent : public Event {
        json j;
        GameInteractionMovementEvent() {}
        GameInteractionMovementEvent(json j_) : j(j_) {}
    };

    // Is dispatched by the renderer and subscribed to by the gui manager. Conveys a button click.
    struct GuiButtonClickedEvent : public Event {
        void* element;
        bool mouseClick;
        GuiButtonClickedEvent(void* element_, bool mouseClick_) : element(element_), mouseClick(mouseClick_) {}
    };

    struct PrintPagesEvent : public Event {
        PrintPagesEvent() {}
    };

    // Is dispatched by the renderer and subscribed to by the game object (?). Notifies the game
    //   that units have been selected.
    struct UnitSelectedEvent : public Event {
        Entity* entity;
        UnitSelectedEvent(Entity* entity_) : entity(entity_) {}
    };

    struct DieAfterAnimationEvent : public Event {
        EntityID id;
        DieAfterAnimationEvent(EntityID id) : id(id) {}
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
