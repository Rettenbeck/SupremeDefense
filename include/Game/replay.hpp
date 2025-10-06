#pragma once

#include <ECS/field.hpp>
#include <Action/action_queue.hpp>
#include <nlohmann/include.hpp>


namespace SupDef {
    
    using ReplayData = std::unordered_map<long, UActionQueue>;

    DEFINE_SCLASS_BEGIN(Replay)
        ReplayData data;
        
        void initialize() { }

        void add(long frameCount, ActionQueue* actionQueue) {
            assert(actionQueue);
            if (actionQueue->getActions().empty()) return;
            auto& aq = data[frameCount];
            if (!aq) aq = std::make_unique<ActionQueue>();
            aq->getActions() = actionQueue->getActions();
        }

        UActionQueue get(long frameCount) {
            auto result = std::make_unique<ActionQueue>();
            if (auto it = data.find(frameCount); it != data.end()) {
                result->getActions() = it->second->getActions();
            }
            return result;
        }

        REFLECT_BEGIN
            REFLECT_MAP_UNIQUE(data, long, ActionQueue)
        REFLECT_END
    DEFINE_SCLASS_END

    DEFINE_UNIQUE(Replay, UReplay);

}
