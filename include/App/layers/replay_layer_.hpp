#pragma once

#include <App/layer.hpp>
#include <Action/action_queue.hpp>


namespace SupDef {

    class ReplayLayer : public Layer {
        private:
            UActionQueue actionQueue;

        public:
            ReplayLayer() {
                priority = PRIORITY_REPLAY;
                actionQueue = std::make_unique<ActionQueue>();
            }
            
            void onAttach() override {
                SUBSCRIBE(ActionsReceivedForGameEvent)
            }
        
            void update(float deltaTime) override {
                // Process any queued events or additional logic if necessary
            }

            DEFINE_EVENT_CALLBACK(ActionsReceivedForGameEvent) {
                assert(actionQueue);
                assert(event.actionQueue);
                actionQueue->clear();
                actionQueue->getActions() = event.actionQueue->getActions();
            }
            
            ActionQueue* getActionQueue() { return actionQueue.get(); }

    };    
    
}
