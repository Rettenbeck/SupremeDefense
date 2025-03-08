#include <Util/basic.hpp>
#include <ECS/constants.hpp>
#include <Action/constants.hpp>
#include <nlohmann/include.hpp>

#pragma once


namespace SupDef {

    class Action {
        public:
            CommandID commandID;
            EntityID entityID;
            EntityID playerID;
            json data;
        
            Action(CommandID commandID, EntityID entityID, EntityID playerID, json data)
                : commandID(commandID), entityID(entityID), playerID(playerID), data(data) { }

            Action() { data = json{}; }
        
            void to_json(json& j) const {
                j = json{
                    {SA_COMMAND_ID, commandID}, {SA_ENTITY_ID, entityID},
                    {SA_PLAYER_ID, playerID}, {SA_DATA, data}
                };
            }
        
            void from_json(const json& j) {
                commandID = j.at(SA_COMMAND_ID).get<CommandID>();
                entityID  = j.at(SA_ENTITY_ID ).get<EntityID>();
                playerID  = j.at(SA_PLAYER_ID ).get<EntityID>();
                data = j.at(SA_DATA);
            }

    };

    using UAction = std::unique_ptr<Action>;
    using Actions = std::vector<UAction>;

    using SAction  = std::shared_ptr<Action>;
    using SActions = std::vector<SAction>;

}
