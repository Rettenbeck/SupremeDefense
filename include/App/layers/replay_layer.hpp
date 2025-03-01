#include <App/layer.hpp>
#include <Action/action_queue.hpp>

#pragma once


namespace SupDef {

    class ReplayLayer : public Layer {
        private:
            UActionQueue actionQueue;

        public:
            ReplayLayer() {
                actionQueue = std::make_unique<ActionQueue>();
            }
            
            void onAttach() override {
                //
            }
        
            void update(float deltaTime) override {
                // Process any queued events or additional logic if necessary
            }

            ActionQueue* getActionQueue() { return actionQueue.get(); }

    };    
    
}
