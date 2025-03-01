#include <Util/basic.hpp>

#pragma once


namespace SupDef {
    
    // Defines
    using PlayerID = uint32_t;
    using PlayerIDs = std::vector<PlayerID>;
    using TeamID = uint32_t;
    using TeamIDs = std::vector<TeamID>;

    const PlayerID NO_PLAYER = 0;
    const TeamID NO_TEAM = 0;

    const std::string SP_PLAYER                             = "player"                              ;
    const std::string SP_PLAYERS                            = "players"                             ;
    const std::string SP_NEXT_PLAYER_ID                     = "next_player_id"                      ;
    const std::string SP_TEAM                               = "team"                                ;
    const std::string SP_TEAMS                              = "teams"                               ;
    const std::string SP_NEXT_TEAM_ID                       = "next_team_id"                        ;

}
