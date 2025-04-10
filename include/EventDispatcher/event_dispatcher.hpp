#pragma once

#include <Util/basic.hpp>
#include <EventDispatcher/events.hpp>


namespace SupDef {
    
    using SubscriptionToken = size_t;
    using SubscriptionTokens = std::vector<SubscriptionToken>;
    using EventCallback = std::function<void(const Event&)>;
    using BatchEventCallback = std::function<void(const Events&)>;
    using EventTypeX = std::type_index;

    class EventDispatcher {
        struct ListenerStruct {
            SubscriptionToken token;
            BatchEventCallback callback;
        };

        using ListenersStruct = std::unordered_map<EventTypeX, std::vector<ListenerStruct>>;
        using BatchedEvents = std::unordered_map<EventTypeX, Events>;
        
        SubscriptionToken nextToken = 1;
        ListenersStruct listeners;
        BatchedEvents batchedEvents;

        public:
            template <typename EventType>
            SubscriptionToken subscribe(BatchEventCallback callback) {
                listeners[typeid(EventType)].push_back({nextToken, [callback](const Events& events) {
                    callback(events);
                }});
                return nextToken++;
            }

            void unsubscribe(SubscriptionToken token) {
                for (auto& [type, listenerList] : listeners) {
                    listenerList.erase(std::remove_if(listenerList.begin(), listenerList.end(),
                                                      [token](const ListenerStruct& l) { return l.token == token; }),
                                      listenerList.end());
                }
            }

            template <typename EventType, typename... Args>
            void dispatch(Args&&... args) {
                EventType event(std::forward<Args>(args)...);
                auto it = listeners.find(typeid(EventType));
                if (it != listeners.end()) {
                    for (const auto& listener : it->second) {
                        Events tempBatch;
                        tempBatch.emplace_back(std::make_unique<EventType>(event));
                        listener.callback(tempBatch);
                    }
                }
            }

            template <typename EventType, typename... Args>
            void queueEvent(Args&&... args) {
                auto event = std::make_unique<EventType>(std::forward<Args>(args)...);
                batchedEvents[typeid(EventType)].push_back(std::move(event));
            }

            void processQueuedEvents() {
                for (auto& [type, events] : batchedEvents) {
                    for (const auto& listener : listeners[type]) {
                        listener.callback(events);
                    }
                    events.clear();
                }
            }

            // template <typename EventType, typename Callback>
            // void subscribeBatch(Callback callback) {
            //     listeners[typeid(EventType)].push_back([callback](const Events& events) {
            //         std::vector<std::reference_wrapper<const EventType>> typedEvents;
            //         for (const auto& event : events) {
            //             typedEvents.push_back(static_cast<const EventType&>(*event));
            //         }
            //         callback(typedEvents);
            //     });
            // }

    };

    using UEventDispatcher = std::unique_ptr<EventDispatcher>;
    
}
