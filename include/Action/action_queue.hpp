#pragma once

#include <Action/action.hpp>


namespace SupDef {

    class ActionQueue {
        SActions actions;
    
        public:
            void enqueue(SAction action) {
                actions.push_back(std::move(action));
            }

            SActions& getActions() {
                return actions;
            }

            void clear() {
                actions.clear();
            }

    };

    using UActionQueue = std::unique_ptr<ActionQueue>;

}
