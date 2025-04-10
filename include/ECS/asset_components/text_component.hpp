#pragma once

#include <ECS/component_registry.hpp>
#include <Misc/text.hpp>


namespace SupDef {

    DEFINE_COMPONENT_BEGIN(TextComponent, SC_TEXT)
        std::unordered_map<std::string, UText> texts;

        TextComponent(std::string key, std::string language, std::string text) {
            addText(key, language, text);
            addToRegistry();
        }

        TextComponent(  std::string key1, std::string language1, std::string text1,
                        std::string key2, std::string language2, std::string text2) {
            addText(key1, language1, text1);
            addText(key2, language2, text2);
            addToRegistry();
        }
        
        void addText(std::string key, UText text) {
            texts[key] = std::move(text);
        }

        void addText(std::string key, std::string language, std::string text) {
            texts[key] = std::make_unique<Text>(language, text);
        }

        std::string get(std::string key, std::string language) {
            auto it = texts.find(key); if (it != texts.end()) return it->second->get(language);
            return NO_TEXT;
        }

        std::string get(std::string key) {
            return get(key, "");
        }

        bool isAsset() const override { return true; }

        REFLECT_COMPONENT_BEGIN(ThisType)
            REFLECT_MAP_UNIQUE(texts, std::string, Text)
        REFLECT_COMPONENT_END()
    DEFINE_COMPONENT_END

}
