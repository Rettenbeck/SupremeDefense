#include <Util/basic.hpp>
#include <Action/action_type.hpp>
#include <Action/constants.hpp>
#include <nlohmann/include.hpp>

#pragma once


namespace SupDef {

    class Action {
        public:
            ActionType type = ActionType::None;
            json data;
        
            Action(ActionType type, json data) : type(type), data(std::move(data)) { }
            Action(ActionType type) : type(type) { data = json{ }; }
            Action() { data = json{ }; }
        
            void to_json(json& j) const {
                j = json{{SA_TYPE, type}, {SA_DATA, data}};
            }
        
            void from_json(const json& j) {
                type = j.at(SA_TYPE).get<ActionType>();
                data = j.at(SA_DATA);
            }

    };

    using UAction = std::unique_ptr<Action>;
    using Actions = std::vector<UAction>;

    using SAction = std::shared_ptr<Action>;
    using SActions = std::vector<SAction>;

}
