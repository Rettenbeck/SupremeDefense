#pragma once

#include <App/Log/message_type.hpp>


namespace SupDef {
    
    class LogMessage {
        public:
            LogMessage(MessageType type, std::string message)
                : type(type), message(std::move(message)), timestamp(std::chrono::system_clock::now()) {}
        
            std::string toString() const {
                std::ostringstream ss;
                ss << "[" << getTimestampString() << "] " << typeToString(type) << ": " << message;
                return ss.str();
            }
        
        private:
            MessageType type;
            std::string message;
            std::chrono::system_clock::time_point timestamp;
        
            std::string getTimestampString() const {
                auto t = std::chrono::system_clock::to_time_t(timestamp);
                std::tm tm{};
        // #ifdef _MSC_VER
                localtime_s(&tm, &t);
        // #else
        //         localtime_r(&t, &tm);
        // #endif
                std::ostringstream oss;
                oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
                return oss.str();
            }
        
            static std::string typeToString(MessageType type) {
                switch (type) {
                    case MessageType::Init   : return "INIT"   ;
                    case MessageType::Info   : return "INFO"   ;
                    case MessageType::Warning: return "WARNING";
                    case MessageType::Error  : return "ERROR"  ;
                    case MessageType::Success: return "SUCCESS";
                    default: return "UNKNOWN";
                }
            }
    };

}