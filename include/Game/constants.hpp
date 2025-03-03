#include <Util/basic.hpp>
#include <ECS/constants.hpp>

#pragma once


namespace SupDef {
    
    using CommandType = std::string;

    const CommandType COM_TYPE_MOVE                         = "move"                                ;
    const CommandType COM_TYPE_BUILD                        = "build"                               ;

    const std::string DEFAULT_FILENAME_SAVEGAME             = "game_save.txt"                       ;

    const std::string SG_GAME                               = "game"                                ;
    const std::string SG_ENTITY_MANAGER                     = "entity_manager"                      ;
    const std::string SG_ASSET_MANAGER                      = "asset_manager"                       ;
    const std::string SG_TECH_MANAGER                       = "tech_manager"                        ;
    // const std::string SG_RULES                              = "rules"                               ;
    // const std::string SG_ASSETS                             = "assets"                              ;
    
    // const std::string SGA_NAME                              = "name"                                ;
    // const std::string SGA_DESCRIPTION                       = "description"                         ;
    // const std::string SGA_COMPONENTS                        = "components"                          ;

}
