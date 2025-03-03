#include <App/Log/logger.hpp>

#pragma once


namespace SupDef {
    
    class Event {
        public:
            virtual ~Event() = default;
    };

    using UEvent = std::unique_ptr<Event>;
    using Events = std::vector<UEvent>;
    
}
