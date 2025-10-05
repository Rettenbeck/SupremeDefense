#pragma once

#include <Util/basic.hpp>


namespace SupDef {
    
    const int PRIORITY_ACTION_ROUTER = 1;
    const int PRIORITY_GAME     = 30;
    const int PRIORITY_REPLAY   = 40;
    const int PRIORITY_GUI      = 50;
    const int PRIORITY_RENDER   = 60;
    const int PRIORITY_NETWORK  = 70;
    const int PRIORITY_ASSET    = 91;

    const unsigned short discoveryPortInitial = 43000;
    const unsigned short connectionPortInitial = 50000;

    const std::string DEFAULT_FILENAME_LOG                  = "log.txt"                             ;
    const std::string DEFAULT_FILENAME_SETTINGS             = "settings.txt"                        ;
    const std::string MESSAGE_INIT                          = "------------------"                  ;

    const std::string S_APP_FRAMERATE                       = "framerate"                           ;
    const std::string S_APP_INIT_WINDOW_WIDTH               = "init_window_width"                   ;
    const std::string S_APP_INIT_WINDOW_HEIGHT              = "init_window_height"                  ;

    const std::string S_NET_MESSAGE_TYPE                    = "network_message_type"                ;
    const std::string S_NET_CHAT                            = "chat"                                ;
    const std::string S_NET_ASSETS                          = "assets"                              ;
    const std::string S_NET_ACTION_FROM_CLIENT              = "action_from_client"                  ;
    const std::string S_NET_ACTIONS_FROM_SERVER             = "actions_from_server"                 ;

    const std::string MOD_FOLDER                            = "mods"                                ;
    const std::string FILETYPE_MOD                          = ".ini"                                ;
    const std::string MOD_HEADER_ENTRY                      = "header"                              ;
    const std::string MOD_HEADER_NAME                       = "name"                                ;
    const std::string MOD_HEADER_DESC                       = "desc"                                ;
    const std::string MOD_HEADER_TEXT_DEFAULT_LANGUAGE      = "en"                                  ;
    const std::string MOD_HEADER_EXCLUSIVE                  = "exclusive"                           ;

}
