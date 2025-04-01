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

            bool assertLog(bool condition, const std::string& message, const char* file, int line, const char* func) {
                if (!condition) {
                    std::stringstream ss;
                    ss << "File: " << file << " Line " << line << "; Function: " << func;
                    if (!message.empty()) ss << ": " << message;
                    addMessage(MessageType::Assert, ss.str());
                }
                return condition;
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


// Internal helper to count args (we only care about 1 or 2)
#define GET_MACRO(_1, _2, NAME, ...) NAME

// Version with message
#define MY_ASSERT2(condition, message) \
    Logger::getInstance().assertLog((condition), (message), __FILE__, __LINE__, __func__)

// Version without message — just use the failed condition string as the message
#define MY_ASSERT1(condition) \
    Logger::getInstance().assertLog((condition), #condition, __FILE__, __LINE__, __func__)

// Dispatcher macro
#define log_assert(...) GET_MACRO(__VA_ARGS__, MY_ASSERT2, MY_ASSERT1)(__VA_ARGS__)
