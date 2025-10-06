#pragma once

#include <EventDispatcher/event.hpp>
#include <ECS/entity.hpp>
#include <App/settings.hpp>
#include <Action/action.hpp>
#include <Action/action_queue.hpp>
#include <Game/mod_data.hpp>


namespace SupDef {

    // #### General events ############################################################################

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
    



    // #### Command events ############################################################################

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




    // #### Gui events ################################################################################

    // Is dispatched by a page object and subscribed to by the page manager. Starts a new page.
    struct GotoPageEvent : public Event {
        int pageTypeId;
        GotoPageEvent(int pageTypeId_) { pageTypeId = pageTypeId_; }
    };

    // Is dispatched by a page object and subscribed to by the page manager. Starts a new page.
    struct PushPageEvent : public Event {
        int pageTypeId;
        PushPageEvent(int pageTypeId_) { pageTypeId = pageTypeId_; }
    };

    // Is dispatched by a page object and subscribed to by the page manager. Closes the top page.
    
    /**
     * @brief Event notifying that the top page is to be closed.
     *
     * Is dispatched by a Page and subscribed to by the PageManager. Closes the top page.
     */
    struct ClosePageEvent : public Event {
        uint32_t pageId = -1;
        ClosePageEvent(uint32_t pageId_) : pageId(pageId_) { }
    };




    // #### Gui menu buttons ##########################################################################

    struct MenuButtonOpenServerEvent : public Event { MenuButtonOpenServerEvent() { } };
    struct MenuButtonJoinServerEvent : public Event { MenuButtonJoinServerEvent() { } };
    struct MenuButtonOpenModListEvent : public Event { MenuButtonOpenModListEvent() { } };




    // #### Game Gui events ###########################################################################
    
    /**
     * @brief Event notifying that a game interaction has been performed.
     *
     * Is dispatched by the renderer and subscribed to by the GuiManager. Contains an event that is
     *   intended for the game logic. This way the gui still has a change about reacting to that
     *   interaction.
     */
    struct GameInteractionEvent : public Event {
        std::function<void()> call;
        GameInteractionEvent() {}
        GameInteractionEvent(std::function<void()> call_) : call(call_) {}
    };

    #define DISPATCH_GIE(DISPATCHER, EVENT, ...) \
        DISPATCHER->dispatch<GameInteractionEvent>( \
            [&]() { DISPATCHER->dispatch<EVENT>(__VA_ARGS__); } \
        );

    /**
     * @brief Event notifying that a movement command has been performed.
     *
     * Is dispatched by the renderer when the player moves a unit and subscribed to by the Game.
     */
    struct GameInteractionMovementEvent : public Event {
        json j;
        GameInteractionMovementEvent() {}
        GameInteractionMovementEvent(json j_) : j(j_) {}
    };

    /**
     * @brief Event notifying that the player has pushed a button.
     *
     * Is dispatched by the renderer when a button is pushed and subscribed to by the GuiManager and
     *   potentially a Page.
     */
    struct GuiButtonClickedEvent : public Event {
        void* element;
        bool mouseClick;
        GuiButtonClickedEvent(void* element_, bool mouseClick_) : element(element_), mouseClick(mouseClick_) {}
    };

    struct PrintPagesEvent : public Event {
        PrintPagesEvent() {}
    };

    /**
     * @brief Event notifying that the player has selected one or more units.
     *
     * Is dispatched by the renderer and subscribed to by the SelectionManager. Notifies that the
     *   player has selected one or more units.
     */
    struct UnitSelectedEvent : public Event {
        Entity* entity;
        UnitSelectedEvent(Entity* entity_) : entity(entity_) {}
    };

    // struct DieAfterAnimationEvent : public Event {
    //     EntityID id;
    //     DieAfterAnimationEvent(EntityID id) : id(id) {}
    // };
    



    // #### Other Gui events ##########################################################################
    
    /**
     * @brief Event notifying that the mod list should be sent.
     *
     * Is dispatched by the ModListPage and subscribed to by the AssetLayer.
     */
    struct RequestModListEvent : public Event { RequestModListEvent() { } };

    struct RequestModListAnswerEvent : public Event {
        Mods* mods;
        RequestModListAnswerEvent(Mods* mods_) : mods(mods_) {}
    };

    /**
     * @brief Event notifying that the mod list should be reprocessed.
     *
     * Is dispatched by the ModListPage and subscribed to by the AssetLayer.
     */
    struct BuildModListEvent : public Event { BuildModListEvent() { } };

    /**
     * @brief Event notifying that the mod list active flags should be written to
     *   the global settings.
     *
     * Is dispatched by the ModListPage and subscribed to by the AssetLayer.
     */
    struct SaveModListEvent : public Event { SaveModListEvent() { } };




    // #### Action events #############################################################################

    /**
     * @brief Event notifying that an action has been performed.
     *
     * Is dispatched by the Game when the player performs an action. Is subscribed to by the
     *   NetworkLayer which in turns distributes it accordingly, and by the ReplayLayer.
     */
    struct ActionCreatedEvent : public Event {
        SAction action;
        ActionCreatedEvent(SAction action_) : action(std::move(action_)) {}
    };

    /**
     * @brief Event to distribute an action list handed out by the server.
     *
     * Is dispatched by the NetworkLayer when all players have responded with their actions. Is
     *   subscribed to by the GameLayer.
     */
    struct ActionReceivedForGameEvent : public Event {
        ActionQueue* actionQueue = nullptr;
        ActionReceivedForGameEvent(ActionQueue* actionQueue) : actionQueue(actionQueue) {}
    };




    // #### Network events ############################################################################

    /**
     * @brief Event notifying that the server list is to be refreshed.
     *
     * Is dispatched by the JoinServerPage when the corresponding button is pushed and is subscribed
     *   to by the SFMLSocketBackend. Notifies the socket backend that a discovery request is be
     *   sent out.
     */
    struct RequestServerListRefreshEvent : public Event { RequestServerListRefreshEvent() {} };

    struct RequestServerListRefreshAnswerEvent : public Event {
        bool ok; std::string message;
        RequestServerListRefreshAnswerEvent(bool ok_, std::string message_) : ok(ok_), message(message_) {}
    };

    /**
     * @brief Event notifying a local server to be started.
     *
     * Is dispatched by the StartPage when the corresponding button is pushed and is subscribed to by
     *   the NetworkLayer. Notifies the network layer that a server is to be opened up.
     */
    struct RequestServerOpenEvent : public Event { RequestServerOpenEvent() {} };
    
    struct RequestServerOpenAnswerEvent : public Event {
        bool ok; std::string message;
        RequestServerOpenAnswerEvent(bool ok_, std::string message_) : ok(ok_), message(message_) {}
    };

    /**
     * @brief Event notifying that a server has been found. Contains the entire server list.
     *
     * Is dispatched by the SFMLBackEnd when a server is found through a discovery request. The event
     *   contains the entire server list since the last discovery reqest. It is subscribed to by the
     *   JoinServerPage.
     */
    struct RetrievedServerListEvent : public Event {
        std::vector<std::string> data;
        RetrievedServerListEvent(std::vector<std::string>& data_) : data(data_) {}
    };

    /**
     * @brief Event notifying the local server to be closed.
     *
     * Is dispatched by the OpenServerPage when the page is closed and is subscribed to by the
     *   NetworkLayer. Notifies that the local server is to be closed.
     */
    struct RequestOpenServerCloseEvent : public Event { RequestOpenServerCloseEvent() {} };
    
    struct RequestOpenServerCloseAnswerEvent : public Event {
        bool ok; std::string message;
        RequestOpenServerCloseAnswerEvent(bool ok_, std::string message_) : ok(ok_), message(message_) {}
    };

    // /**
    //  * @brief Event notifying the player to start looking for servers to join.
    //  *
    //  * Is dispatched by the JoinServerPage when the page is closed and is subscribed to by the
    //  *   NetworkLayer. Notifies that the a discovery request is to be done.
    //  */
    // struct PrepareForDiscoveryEvent : public Event { PrepareForDiscoveryEvent() {} };
    
    // struct PrepareForDiscoveryAnswerEvent : public Event {
    //     bool ok; std::string message;
    //     PrepareForDiscoveryAnswerEvent(bool ok_, std::string message_) : ok(ok_), message(message_) {}
    // };

    /**
     * @brief Event notifying the player is no longer looking for other servers.
     *
     * Is dispatched by the JoinServerPage when the page is closed and is subscribed to by the
     *   NetworkLayer. Notifies that the discoveries are no longer needed.
     */
    struct PrepareForDiscoveryEndEvent : public Event { PrepareForDiscoveryEndEvent() {} };




    struct SetPlayerNameEvent : public Event {
        std::string name;
        SetPlayerNameEvent(std::string name) : name(name) {}
    };

    /**
     * @brief Event notifying the local server to be closed.
     *
     * Is dispatched by the NetworkLayer when the game needs to be paused to wait for other players'
     *   connections. Is subscribed to by the Game Layer and causes it to pause.
     */
    struct GameBlockedByNetworkEvent : public Event {
        bool blocked;
        GameBlockedByNetworkEvent(bool blocked = true) : blocked(blocked) {}
    };

    /**
     * @brief Event notifying that the Game has updated by one frame.
     *
     * Is dispatched by the GameLayer when after it has processed one frame.
     */
    struct GameHasUpdatedEvent : public Event {
        EntityID thisPlayer;
        long frameCount;
        GameHasUpdatedEvent(EntityID thisPlayer, long frameCount)
        : thisPlayer(thisPlayer), frameCount(frameCount) {}
    };

    /**
     * @brief Event containing the current in-game player list.
     *
     * Is dispatched by the Game and is subscribed to by the NetworkLayer. Notifies the network layer
     *   that a server is to be opened up.
     */
    struct SendPlayerListEvent : public Event {
        EntityIDs playerList;
        SendPlayerListEvent(EntityIDs& playerList) : playerList(playerList) {}
    };

}
