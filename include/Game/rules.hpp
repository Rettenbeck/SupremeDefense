#include <Util/basic.hpp>
#include <nlohmann/include.hpp>

#pragma once


namespace SupDef {

    class Rules {
        public:
            using RuleValue = std::variant<int, float, bool, std::string>;

        private:
            std::unordered_map<std::string, RuleValue> values;

        public:
            void set(const std::string& key, RuleValue value) {
                values[key] = std::move(value);
            }

            template <typename T>
            std::optional<T> get(const std::string& key) const {
                auto it = values.find(key);
                if (it != values.end()) {
                    if (auto val = std::get_if<T>(&it->second)) {
                        return *val;
                    }
                }
                return std::nullopt;
            }

            void to_json(json& j) const {
                for (const auto& [key, value] : values) {
                    std::visit([&j, &key](auto&& v) {
                        j[key] = v;
                    }, value);
                }
            }

            void from_json(const json& j) {
                for (auto it = j.begin(); it != j.end(); ++it) {
                    if (it->is_number_float()) values[it.key()] = it->get<float>();
                    else if (it->is_number_integer()) values[it.key()] = it->get<int>();
                    else if (it->is_boolean()) values[it.key()] = it->get<bool>();
                    else if (it->is_string()) values[it.key()] = it->get<std::string>();
                }
            }
            
    };

    using URules = std::unique_ptr<Rules>;

}
