#pragma once

#include <Util/basic.hpp>
#include <nlohmann/include.hpp>
#include <App/Log/logger.hpp>


namespace SupDef {

    class Settings {
        public:

            void to_json(json& j) const { j = data; }
            void from_json(const json& j) { data = j; }
        
            bool loadFromFile(const std::string& filename) {
                std::ifstream file(filename);
                if (!file) {
                    std::stringstream ss;
                    ss << "Failed to open settings file: " << filename;
                    LOG_ERROR(ss.str())
                    return false;
                }
                try {
                    json j;
                    file >> j;
                    from_json(j);
                } catch (const nlohmann::json::parse_error& e) {
                    std::stringstream ss;
                    ss << "JSON parsing error: " << e.what();
                    LOG_ERROR(ss.str())
                    return false;
                }
                return true;
            }
        
            bool saveToFile(const std::string& filename) const {
                std::ofstream file(filename);
                if (!file) {
                    std::stringstream ss;
                    ss << "Failed to open settings file: " << filename;
                    LOG_ERROR(ss.str())
                    return false;
                }
                json j;
                to_json(j);
                file << j.dump(4);
                return true;
            }
        
            template <typename T>
            void set(const std::string& key, const T& value) {
                data[key] = value;
            }
        
            template <typename T>
            T get(const std::string& key, const T& defaultValue) const {
                if (data.contains(key)) {
                    try {
                        return data.at(key).get<T>();
                    } catch (const nlohmann::json::type_error&) {
                        std::stringstream ss;
                        ss << "Type mismatch for key: " << key;
                        LOG_ERROR(ss.str())
                    }
                }
                return defaultValue;
            }
        
            Settings() {}
            Settings(const std::string& filename) { loadFromFile(filename); }
    
            json& raw() { return data; }
            const json& raw() const { return data; }
        
        private:
            json data;

    };

    using USettings = std::unique_ptr<Settings>;

}
