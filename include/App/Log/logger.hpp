#pragma once

#include <App/Log/log_message.hpp>


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
#define GET_MACRO(_1, _2, _3, NAME, ...) NAME

// Version with message
#define MY_ASSERT3(condition, message, unused) \
    Logger::getInstance().assertLog((condition), (message), __FILE__, __LINE__, __func__)

// Version with message
#define MY_ASSERT2(condition, message) \
    Logger::getInstance().assertLog((condition), (message), __FILE__, __LINE__, __func__)

// Version without message — just use the failed condition string as the message
#define MY_ASSERT1(condition) \
    Logger::getInstance().assertLog((condition), #condition, __FILE__, __LINE__, __func__)

// Dispatcher macro
#define log_assert(...) GET_MACRO(__VA_ARGS__, MY_ASSERT3, MY_ASSERT2, MY_ASSERT1)(__VA_ARGS__)



#define LOG_1(TYPE) \
Logger::getInstance().addMessage(MessageType::TYPE, "");

#define LOG_2(TYPE, MESSAGE) \
Logger::getInstance().addMessage(MessageType::TYPE, MESSAGE);

#define LOG_3(TYPE, MESSAGE, ARG1) \
Logger::getInstance().addMessage(MessageType::TYPE, MESSAGE, ARG1);

#define LOG(...) GET_MACRO(__VA_ARGS__, LOG_3, LOG_2, LOG_1)(__VA_ARGS__)



#define LOG_ERROR_3(MESSAGE, ARG1, unused) \
LOG(Error, MESSAGE, ARG1)

#define LOG_ERROR_2(MESSAGE, ARG1) \
LOG(Error, MESSAGE, ARG1)

#define LOG_ERROR_1(MESSAGE) \
LOG(Error, MESSAGE)

#define LOG_ERROR(...) \
GET_MACRO(__VA_ARGS__, LOG_ERROR_3, LOG_ERROR_2, LOG_ERROR_1)(__VA_ARGS__)



#define LOG_INFO_3(MESSAGE, ARG1, unused) \
LOG(Info, MESSAGE, ARG1)

#define LOG_INFO_2(MESSAGE, ARG1) \
LOG(Info, MESSAGE, ARG1)

#define LOG_INFO_1(MESSAGE) \
LOG(Info, MESSAGE)

#define LOG_INFO(...) \
GET_MACRO(__VA_ARGS__, LOG_INFO_3, LOG_INFO_2, LOG_INFO_1)(__VA_ARGS__)




#define JSON_CONTAINS_WITH_MSG_AND_RETURN_(JSON, FIELD, MESSAGE, ARG1) \
CHECK_CONDITION_WITH_MSG_AND_RETURN(!JSON.contains(FIELD), MESSAGE, ARG1)

#define JSON_CONTAINS_WITH_MSG_AND_RETURN(JSON, FIELD, MESSAGE) \
CHECK_CONDITION_WITH_MSG_AND_RETURN(!JSON.contains(FIELD), MESSAGE)


#define CHECK_CONDITION_WITH_MSG_AND_RETURN_3(CONDITION, MESSAGE, ARG1) \
if (CONDITION) { \
    LOG_ERROR(MESSAGE, ARG1) \
    return; \
}

#define CHECK_CONDITION_WITH_MSG_AND_RETURN_2(CONDITION, MESSAGE) \
if (CONDITION) { \
    LOG_ERROR(MESSAGE) \
    return; \
}

#define CHECK_CONDITION_WITH_MSG_AND_RETURN_1(CONDITION) \
if (CONDITION) { \
    LOG_ERROR("[No message]") \
    return; \
}

#define CHECK_CONDITION_WITH_MSG_AND_RETURN(...) \
GET_MACRO(__VA_ARGS__, CHECK_CONDITION_WITH_MSG_AND_RETURN_3, CHECK_CONDITION_WITH_MSG_AND_RETURN_2, CHECK_CONDITION_WITH_MSG_AND_RETURN_1)(__VA_ARGS__)

