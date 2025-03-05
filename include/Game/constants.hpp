#include <Util/basic.hpp>
#include <ECS/constants.hpp>

#pragma once


namespace SupDef {
    
    using CommandType = std::string;

    const std::string DEFAULT_FILENAME_SAVEGAME             = "game_save.txt"                       ;

    const std::string SG_GAME                               = "game"                                ;
    const std::string SG_ENTITY_MANAGER                     = "entity_manager"                      ;
    const std::string SG_ASSET_MANAGER                      = "asset_manager"                       ;
    const std::string SG_TECH_MANAGER                       = "tech_manager"                        ;

}
