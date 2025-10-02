#pragma once

#include <EventDispatcher/event_dispatcher.hpp>


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
            void subscribe(BatchEventCallback callback) {
                assert(globalDispatcher);
                auto token = globalDispatcher->subscribe<EventType>(callback);
                tokens.push_back(token);
            }

            template <typename EventType, typename... Args>
            void dispatch(Args&&... args) {
                assert(globalDispatcher);
                globalDispatcher->dispatch<EventType>(std::forward<Args>(args)...);
            }

    };

    using UListener = std::unique_ptr<Listener>;

    #define DEFINE_EVENT_CALLBACK_BEGIN(EVENT) void on##EVENT(const EVENT& event)
    #define DEFINE_EVENT_CALLBACK_SHORT_BEGIN(EVENT) void on##EVENT()

    #define SUBSCRIBE2(EVENT, FUNC) \
    subscribe<EVENT>([this](const SupDef::Events& events) { \
        for (const auto& event_ : events) {                 \
            const auto& event = static_cast<const EVENT&>(*event_); \
            FUNC(event); \
        } \
    });
    #define SUBSCRIBE1(EVENT) SUBSCRIBE2(EVENT, on##EVENT)
    #define SUBSCRIBE_GET(_1, _2, NAME, ...) NAME
    #define SUBSCRIBE(...) SUBSCRIBE_GET(__VA_ARGS__, SUBSCRIBE2, SUBSCRIBE1)(__VA_ARGS__)

    #define SUBSCRIBE_SHORT2(EVENT, FUNC) \
    subscribe<EVENT>([this](const SupDef::Events& events) { \
        for (const auto& event_ : events) {                 \
            FUNC(); \
        } \
    });
    #define SUBSCRIBE_SHORT1(EVENT) SUBSCRIBE_SHORT2(EVENT, on##EVENT)
    #define SUBSCRIBE_SHORT_GET(_1, _2, NAME, ...) NAME
    #define SUBSCRIBE_SHORT(...) SUBSCRIBE_SHORT_GET(__VA_ARGS__, SUBSCRIBE_SHORT2, SUBSCRIBE_SHORT1)(__VA_ARGS__)




    #define SUBSCRIBE_BEGIN(DISPATCHER, EVENT) \
    assert(DISPATCHER); \
    DISPATCHER->subscribe<EVENT>([this](const SupDef::Events& events) { \
        for (const auto& event : events) {                              \
            const auto& typedEvent = static_cast<const EVENT&>(*event);
    
    #define SUBSCRIBE_END \
        } \
    });

    #define SUBSCRIBE_ACTION_BEGIN(DISPATCHER, EVENT, ACTIONS) \
    SUBSCRIBE_BEGIN(DISPATCHER, EVENT) \
    ACTIONS.push_back([&]() {

    #define SUBSCRIBE_ACTION_END \
    }); \
    SUBSCRIBE_END


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

    #define SUBSCRIBE_SIMPLE(DISPATCHER, EVENT, FUNC) \
        SUBSCRIBE_BEGIN(DISPATCHER, EVENT) \
            FUNC; \
        SUBSCRIBE_END

    #define SUBSCRIBE_ACTION_SIMPLE(DISPATCHER, EVENT, ACTIONS, FUNC) \
        SUBSCRIBE_ACTION_BEGIN(DISPATCHER, EVENT, ACTIONS) \
            FUNC; \
        SUBSCRIBE_ACTION_END

}
