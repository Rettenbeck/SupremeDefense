#include <EventDispatcher/event_dispatcher.hpp>
#include <App/timer.hpp>

#pragma once


namespace SupDef {

    class Layer : public Listener , public Timer {
        protected:
            int priority = 0;
        
        public:
            virtual ~Layer() = default;
            
            virtual void onAttach() { }
            virtual void onDetach() { }
            virtual void onStart () { }
            virtual void update(float deltaTime) = 0;

            int getPriority() const { return priority; }
            void setPriority(int priority_) { priority = priority_; }

    };
     
    using ULayer = std::unique_ptr<Layer>;
    using Layers = std::vector<ULayer>;
   
}
