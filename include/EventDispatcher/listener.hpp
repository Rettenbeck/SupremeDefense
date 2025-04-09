#include <EventDispatcher/event_dispatcher.hpp>

#pragma once


namespace SupDef {
        
    class Listener {
        protected:
            SubscriptionTokens tokens;
            EventDispatcher* globalDispatcher = nullptr;
    
        public:
            Listener() {}
        
            void setGlobalDispatcher(EventDispatcher* globalDispatcher_) {
                globalDispatcher = globalDispatcher_;
            }

            EventDispatcher* getGlobalDispatcher() { return globalDispatcher; }

            virtual ~Listener() {
                if (globalDispatcher) {
                    for (auto token : tokens) {
                        globalDispatcher->unsubscribe(token);
                    }
                }
            }
        
            template <typename EventType>
            void subscribeGlobal(BatchEventCallback callback) {
                auto token = globalDispatcher->subscribe<EventType>(callback);
                tokens.push_back(token);
            }

    };

    #define SUBSCRIBE_BEGIN(DISPATCHER, EVENT) \
    assert(DISPATCHER); \
    DISPATCHER->subscribe<EVENT>([this](const SupDef::Events& events) { \
        for (const auto& event : events) {                              \
            const auto& typedEvent = static_cast<const EVENT&>(*event);
    
    #define SUBSCRIBE_END \
        } \
    });


    #define SUBSCRIBE_HEAD_BEGIN(DISPATCHER, EVENT) \
    { \
    using ThisEventType = EVENT; \
    assert(DISPATCHER); \
    DISPATCHER->subscribe<ThisEventType>([this](const SupDef::Events& events) {

    #define SUBSCRIBE_BODY_BEGIN \
        for (const auto& event : events) { \
            const auto& typedEvent = static_cast<const ThisEventType&>(*event);
    
    #define SUBSCRIBE_BODY_END \
        }
    
    #define SUBSCRIBE_HEAD_END \
    }); \
    }


}
