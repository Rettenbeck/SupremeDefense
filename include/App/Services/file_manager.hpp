#pragma once

#include <EventDispatcher/event_dispatcher.hpp>
#include <App/constants.hpp>


namespace SupDef {

    class FileManager : public Listener {
        public:
            FileManager(EventDispatcher* globalDispatcher_) {
                assert(globalDispatcher_);
                setGlobalDispatcher(globalDispatcher_);
            }
        
    };
     
    DEFINE_UNIQUE(FileManager, UFileManager)

}
