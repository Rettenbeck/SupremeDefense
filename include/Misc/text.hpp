#include <Util/basic.hpp>

#pragma once


namespace SupDef {

    const std::string NO_TEXT = "no text found";

    class Text {
        private:
            std::unordered_map<std::string, std::string> texts;

        public:
            Text() {}
            Text(std::string language, std::string text) {
                texts[language] = text;
            }

            std::string get(std::string language) {
                if (texts.empty()) return NO_TEXT;
                auto it = texts.find(language); if (it != texts.end()) return it->second;
                it = texts.find("en"); if (it != texts.end()) return it->second;
                it = texts.find("de"); if (it != texts.end()) return it->second;
                for (auto [lang, text] : texts) return text;
                return NO_TEXT;
            }

            std::string get() { return get(""); }

            void to_json(json& j) const {
                generic_to_json(j, this);
            }
    
            void from_json(const json& j) {
                generic_from_json(j, this);
            }
    
            REFLECT_COMPONENT_BEGIN(Text)
                REFLECT_FIELD(texts)
            REFLECT_COMPONENT_END()
    
            // void to_json(json& j) const {
            //     j = texts;
            // }
            
            // void from_json(const json& j) {
            //     texts = j.get<std::unordered_map<std::string, std::string>>();
            // }
        
    };

    using UText = std::unique_ptr<Text>;

}
