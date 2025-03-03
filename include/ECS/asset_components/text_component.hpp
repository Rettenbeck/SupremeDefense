#include <ECS/component_registry.hpp>
#include <Util/text.hpp>

#pragma once


namespace SupDef {

    struct TextComponent : public Component {
        std::unordered_map<std::string, UText> texts;

        TextComponent() { addToRegistry(); }

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
        
        void addToRegistry() {
            ComponentRegistry::registerComponent(getTypeName(), []()
                { return std::make_unique<TextComponent>(); });
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
            return get(key);
        }

        void to_json(json& j) const override {
            for (const auto& [key, text] : texts) {
                if (text) {
                    json textJson;
                    text->to_json(textJson);
                    textJson[S_KEY] = key;
                    j[S_TEXTS].push_back(textJson);
                }
            }
        }
    
        void from_json(const json& j) override {
            texts.clear();
            if (j.contains(S_TEXTS) && j[S_TEXTS].is_array()) {
                for (const auto& textJson : j[S_TEXTS]) {
                    auto key = textJson.at(S_KEY).get<std::string>();
                    auto text = std::make_unique<Text>();
                    text->from_json(textJson);
                    texts[key] = std::move(text);
                }
            }
        }
    
        std::string getTypeName() const override {
            return SC_TEXT;
        }

        bool isAssetOnly() const override { return true; }

    };
    
}
