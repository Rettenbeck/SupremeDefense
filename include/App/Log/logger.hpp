#include <App/Log/log_message.hpp>

#pragma once


namespace SupDef {

    class Logger {
        public:
            static Logger& getInstance() {
                static Logger instance;
                return instance;
            }
        
            template<typename... Args>
            void addMessage(MessageType type, std::string message, Args... args) {
                std::string formattedMessage = formatMessage(std::move(message), args...);
                messages.emplace_back(type, formattedMessage);
        
                if (consoleOutput) {
                    std::cout << messages.back().toString() << std::endl;
                }
                if (fileOutput) {
                    fileOutput << messages.back().toString() << std::endl;
                }
            }
        
            void setFileOutput(const std::string& filename) {
                fileOutput.open(filename, std::ios::app);
            }
        
            void enableConsoleOutput(bool enable) {
                consoleOutput = enable;
            }
        
        private:
            std::vector<LogMessage> messages;
            std::ofstream fileOutput;
            bool consoleOutput = true;
        
            Logger() = default;
            ~Logger() { if (fileOutput.is_open()) fileOutput.close(); }
        
            template<typename T, typename... Args>
            std::string formatMessage(std::string message, T value, Args... args) {
                size_t pos = message.find("&1");
                if (pos != std::string::npos) {
                    std::ostringstream ss;
                    ss << value;
                    message.replace(pos, 2, ss.str());
                }
                return formatMessage(message, args...);
            }
        
            std::string formatMessage(std::string message) {
                return message;
            }
    };
    
}