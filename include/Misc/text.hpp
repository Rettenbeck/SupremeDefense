#pragma once

#include <Util/basic.hpp>


namespace SupDef {

    const std::string NO_TEXT = "no text found";

    DEFINE_SCLASS_BEGIN(Text)
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

        REFLECT_COMPONENT_BEGINX
            REFLECT_FIELD(texts)
        REFLECT_COMPONENT_END()

        private:
            std::unordered_map<std::string, std::string> texts;
    DEFINE_SCLASS_END

    DEFINE_UNIQUE(Text, UText);

}
