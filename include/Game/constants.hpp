#include <Util/basic.hpp>
#include <ECS/constants.hpp>

#pragma once


namespace SupDef {
    
    using AssetID = std::string;

    const std::string DEFAULT_FILENAME_SAVEGAME             = "game_save.txt"                       ;

    const std::string SG_GAME                               = "game"                                ;
    const std::string SG_PLAYER_MANAGER                     = "player_manager"                      ;
    const std::string SG_ENTITY_MANAGER                     = "entity_manager"                      ;
    const std::string SG_TECH_MANAGER                       = "tech_manager"                        ;
    const std::string SG_RULES                              = "rules"                               ;
    const std::string SG_ASSETS                             = "assets"                              ;
    
    const std::string SGA_NAME                              = "name"                                ;
    const std::string SGA_DESCRIPTION                       = "description"                         ;
    const std::string SGA_COMPONENTS                        = "components"                          ;

}
