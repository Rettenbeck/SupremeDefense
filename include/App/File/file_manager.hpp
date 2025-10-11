#pragma once

#include <EventDispatcher/event_dispatcher.hpp>
// #include <App/timer.hpp>
// #include <App/settings.hpp>
#include <App/constants.hpp>


namespace SupDef {

    class FileManager : public Listener {
        public:
            static FileManager& getInstance() {
                static FileManager instance;
                return instance;
            }

            FileManager(const FileManager&) = delete;
            FileManager(FileManager&&) = delete;
            FileManager& operator=(const FileManager&) = delete;
            FileManager& operator=(FileManager&&) = delete;

            void doSomething() {
                std::cout << "Singleton instance at " << this << "\n";
            }

        private:
            Singleton() { }
            ~Singleton() { }

    };
     
    using ULayer = std::unique_ptr<Layer>;
    using Layers = std::vector<ULayer>;
   
}
