#pragma once

#include <ECS/field.hpp>
#include <Action/action.hpp>


namespace SupDef {

    DEFINE_SCLASS_BEGIN(ActionQueue)
        ActionQueue(std::unique_ptr<ActionQueue> actionQueue) {
            assert(actionQueue);
            actions = actionQueue->getActions();
        }

        void enqueue(SAction action) {
            actions.push_back(std::move(action));
        }

        SActions& getActions() {
            return actions;
        }

        void clear() {
            actions.clear();
        }

        REFLECT_BEGIN
            REFLECT_LIST_SHARED(actions, Action)
        REFLECT_END

        private:
            SActions actions;

    DEFINE_SCLASS_END

    using UActionQueue = std::unique_ptr<ActionQueue>;

}
