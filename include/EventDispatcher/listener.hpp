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

}
