#include <Util/util.hpp>
#include <EventDispatcher/include.hpp>
#include <App/Log/logger.hpp>

#pragma once


namespace SupDef {

    struct NetworkPlayerData {
        long waiting = 0;
        long frameCount = -1;
        bool received = false;
        SActions actions;
        NetworkPlayerData() {}
        NetworkPlayerData(long waiting, bool received, SActions actions)
        : waiting(waiting), received(received), actions(actions) {}
    };

    class NetworkPlayerTracker {
        public:
            NetworkPlayerTracker() {}

            void resetPlayerList(std::vector<uint32_t> players_) {
                players = players_;
                playerMap.clear();
                for (auto p : players) {
                    playerMap[p] = NetworkPlayerData();
                }
            }

            void addPlayerMessage(json& j) {
                uint32_t currentPlayer = 0;
                JSON_CONTAINS_WITH_MSG_AND_RETURN(j, SA_PLAYER, "Client message does not contain player data");
                currentPlayer = j[SA_PLAYER];
                CHECK_CONDITION_WITH_MSG_AND_RETURN(!playerMap.count(currentPlayer), "Client message of player &1 contains not existing player number", currentPlayer)
                CHECK_CONDITION_WITH_MSG_AND_RETURN(currentPlayer < 1, "Client message of player &1 does not contain incorrect player data", currentPlayer)
                JSON_CONTAINS_WITH_MSG_AND_RETURN_(j, SA_FRAME_COUNT, "Client message of player &1 does not contain frame data", currentPlayer);
                // JSON_CONTAINS_WITH_MSG_AND_RETURN(j, SA_ACTIONS, "Client message of player &1 does not contain action data", currentPlayer);

                auto& data = playerMap[currentPlayer];
                CHECK_CONDITION_WITH_MSG_AND_RETURN(data.received, "Client message of player &1 has sent a duplicate", currentPlayer)
                
                data.received = true;
                data.frameCount = j[SA_FRAME_COUNT];
                data.actions.clear();
                CHECK_CONDITION_WITH_MSG_AND_RETURN(data.frameCount != gameFrameCount, "Client message of player &1 has sent wrong frame count", currentPlayer)

                if (j.contains(SA_ACTIONS)) {
                    for (auto& actionJson : j[SA_ACTIONS]) {
                        auto action = std::make_shared<Action>();
                        action->from_json(actionJson);
                        data.actions.push_back(action);
                    }
                }
            }

            bool complete() {
                for(auto& [player, data] : playerMap) {
                    if (!data.received) return false;
                }
                return true;
            }

            void to_json(json& j) {
                j[S_NET_MESSAGE_TYPE] = S_NET_ACTIONS_FROM_SERVER;
                j[SA_FRAME_COUNT] = gameFrameCount;
                for(auto& [player, data] : playerMap) {
                    // j[SA_PLAYER] = player;
                    for (auto& action : data.actions) {
                        json actionJson = json();
                        action->to_json(actionJson);
                        j[SA_ACTIONS].push_back(actionJson);
                    }
                }
            }

            void advanceWaitingTime() {
                for(auto& [player, data] : playerMap) {
                    if (!data.received) data.waiting++;
                }
            }

            void setFrameCount(long gameFrameCount_) { gameFrameCount = gameFrameCount_; }
            void setThisPlayer(uint32_t thisPlayer_) { thisPlayer = thisPlayer_; }
            uint32_t getThisPlayer() { return thisPlayer; }

        private:
            uint32_t thisPlayer = 0;
            std::vector<uint32_t> players;
            std::unordered_map<uint32_t, NetworkPlayerData> playerMap;
            long gameFrameCount = -1;
            
    };

    using UNetworkPlayerTracker = std::unique_ptr<NetworkPlayerTracker>;

}
