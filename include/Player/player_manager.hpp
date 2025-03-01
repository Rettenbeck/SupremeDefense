#include <Player/player.hpp>
#include <Player/team.hpp>

#pragma once


namespace SupDef {

    class PlayerManager {
        private:
            EventDispatcher* eventDispatcher = nullptr;
            std::unordered_map<PlayerID, UPlayer> players;
            std::unordered_map<TeamID, UTeam> teams;
            PlayerID nextPlayerID = 1;
            TeamID nextTeamID = 1;
        
        public:
            PlayerManager(EventDispatcher* eventDispatcher_) : eventDispatcher(eventDispatcher_) {}
            PlayerManager() {}

            PlayerID createPlayer() {
                auto id = nextPlayerID++;
                players[id] = std::make_unique<Player>(id);
                return id;
            }
        
            PlayerID createPlayer(TeamID teamID) {
                auto id = createPlayer();
                assignPlayerToTeam(id, teamID);
                return id;
            }
        
            TeamID createTeam(PlayerID initialPlayerID = NO_PLAYER) {
                auto id = nextTeamID++;
                teams[id] = std::make_unique<Team>(id);
                if (initialPlayerID != NO_PLAYER) assignPlayerToTeam(initialPlayerID, id);
                return id;
            }
        
            void assignPlayerToTeam(PlayerID playerID, TeamID teamID) {
                int oldTeamID = players[playerID]->teamID;
                if (oldTeamID != NO_TEAM) {
                    auto oldTeam = teams[oldTeamID].get();
                    PlayerIDs& vec = oldTeam->players;
                    vec.erase(std::remove(vec.begin(), vec.end(), playerID), vec.end());
                }
                players[playerID]->teamID = teamID;
                teams[teamID]->players.push_back(playerID);
                //teams[teamID]->isIndependent = (teams[teamID].playerIDs.size() == 1);
            }
        
            int getPlayerTeam(PlayerID playerID) const {
                return players.at(playerID)->teamID;
            }
        
            Player* getPlayer(PlayerID playerID) { return players[playerID].get(); }
            Team* getTeam(TeamID teamID) { return teams[teamID].get(); }

            void to_json(json& j) const {
                j[SP_PLAYERS] = json::array();
                j[SP_TEAMS] = json::array();
            
                for (const auto& [id, player] : players) {
                    json playerJson;
                    player->to_json(playerJson);
                    j[SP_PLAYERS].push_back(playerJson);
                }
            
                for (const auto& [id, team] : teams) {
                    json teamJson;
                    team->to_json(teamJson);
                    j[SP_TEAMS].push_back(teamJson);
                }
            
                j[SP_NEXT_PLAYER_ID] = nextPlayerID;
                j[SP_NEXT_TEAM_ID] = nextTeamID;
            }
            
            void from_json(const json& j) {
                players.clear();
                teams.clear();
            
                for (const auto& playerJson : j[SP_PLAYERS]) {
                    PlayerID id = playerJson[S_ID].get<PlayerID>();
                    auto player = std::make_unique<Player>(id);
                    player->from_json(playerJson);
                    players[id] = std::move(player);
                }
            
                for (const auto& teamJson : j[SP_TEAMS]) {
                    TeamID id = teamJson[S_ID].get<TeamID>();
                    auto team = std::make_unique<Team>(id);
                    team->from_json(teamJson);
                    teams[id] = std::move(team);
                }

                nextPlayerID = j[SP_NEXT_PLAYER_ID];
                nextTeamID = j[SP_NEXT_TEAM_ID];
            }            

    };

    using UPlayerManager = std::unique_ptr<PlayerManager>;

}
