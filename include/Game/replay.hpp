#pragma once

#include <EventDispatcher/event_dispatcher.hpp>
#include <Action/action_queue.hpp>
#include <nlohmann/include.hpp>


namespace SupDef {
    
    using ReplayData = std::unordered_map<long, SActions>;

    class Replay : public Listener {
        public:
            Replay() { }

            void initialize() { }



        private:
            ReplayData data;
    };
    
    using UReplay = std::unique_ptr<Replay>;

}
