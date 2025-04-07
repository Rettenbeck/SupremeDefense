#include <Util/util.hpp>
#include <EventDispatcher/include.hpp>
#include <App/Log/logger.hpp>

#pragma once


namespace SupDef {

    struct NetworkPlayerData {
        long waiting = 0;
        long frameCount = -1;
        bool received = false;
        json actions = json();
        NetworkPlayerData() {}
        NetworkPlayerData(long waiting, bool received, json actions)
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
                CHECK_CONDITION_WITH_MSG_AND_RETURN(currentPlayer < 1, "Client message does not contain incorrect player data")

                // for (const auto& action : actionQueue->getActions()) {
                //     json actionJson;
                //     action->to_json(actionJson);
                //     j[SA_ACTIONS].push_back(actionJson);
                // }
                // j[SA_PLAYER] = thisPlayer;
                // j[SA_FRAME_COUNT] = gameFrameCount;
            }

            void setThisPlayer(uint32_t thisPlayer_) { thisPlayer = thisPlayer_; }
            uint32_t getThisPlayer() { return thisPlayer; }

        private:
            uint32_t thisPlayer = 0;
            std::vector<uint32_t> players;
            std::unordered_map<uint32_t, NetworkPlayerData> playerMap;
            
    };

    using UNetworkPlayerTracker = std::unique_ptr<NetworkPlayerTracker>;

}
