#include <EventDispatcher/event_dispatcher.hpp>

#pragma once


namespace SupDef {

    template <typename EventType>
    class FilteredEventListener {
        std::function<bool(const EventType&)> filter;
        std::function<void(const EventType&)> callback;
        
        public:
            FilteredEventListener(std::function<void(const EventType&)> cb, std::function<bool(const EventType&)> flt)
                : callback(cb), filter(flt) { }
        
            void handleEvent(const EventType& event) {
                if (filter(event)) {
                    callback(event);
                }
            }

    };
    
}
