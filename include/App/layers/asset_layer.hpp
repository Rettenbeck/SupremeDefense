#pragma once

#include <App/layer.hpp>
#include <Game/game.hpp>
#include <Game/build_assets.hpp>


namespace SupDef {

    struct FileData {
        std::string filename = "";
        json j;
    };
    DEFINE_UNIQUE_AND_LIST(FileData, UFile, Files);

    struct ModData {
        std::string defaultname = "", path = "";
        UText name, desc;
        bool exclusive = false;
        bool active = true;
        Files files;

        std::string getName() {
            if (name) return name->get();
            return defaultname;
        }
        std::string getActive() {
            return (active ? "y" : "n");
        }
    };
    DEFINE_UNIQUE_AND_LIST(ModData, UMod, Mods);

    class AssetLayer : public Layer {
        private:
            UAssetManager originalAssetManager, assetManager;
            Mods mods;

        public:
            AssetLayer() {
                priority = PRIORITY_ASSET;
                
                originalAssetManager = std::make_unique<AssetManager>();
                assetManager = std::make_unique<AssetManager>();
            }
            
            void onAttach() override {
                assert(originalAssetManager);
                assert(assetManager);
                BuildAssets::build(originalAssetManager.get());
                BuildAssets::build(assetManager.get());
                mods.clear();
                
                SUBSCRIBE(RetrieveModListEvent)
            }
        
            void update(float deltaTime) override {
                // Process any queued events or additional logic if necessary
            }

            DEFINE_EVENT_CALLBACK(RetrieveModListEvent) {
                retrieveMods();
                // printModList();
                applyMods();
                sendModList();
            }

            void sendModList() {
                std::vector<std::string> data;
                for(auto& mod : mods) {
                    data.push_back(mod->getName());
                    data.push_back(mod->desc->get());
                    data.push_back(mod->getActive());
                    data.push_back(mod->defaultname);
                    data.push_back(mod->path);
                }
                dispatch<RetrieveModListAnswerEvent>(data);
            }

            void applyMods() {
                json j_am;
                assert(assetManager);
                assert(originalAssetManager);

                for(auto& mod : mods) {
                    if (mod->active) {
                        for(auto& file : mod->files) {
                            j_am.merge_patch(file->j);
                        }
                    }
                }
            }

            void retrieveMods() {
                mods.clear();
                namespace fs = std::filesystem;
                fs::path modPath = fs::current_path() / MOD_FOLDER;

                try {
                    for (const auto& entry : fs::directory_iterator(modPath)) {
                        if (entry.is_directory()) {
                            auto mod = std::make_unique<ModData>();
                            auto modFolderPathRel = entry.path().lexically_relative(fs::current_path());
                            mod->path = modFolderPathRel.string();
                            mod->defaultname = entry.path().filename().string();

                            for (const auto& fileEntry : fs::directory_iterator(entry.path())) {
                                if (fileEntry.is_regular_file() && fileEntry.path().extension() == FILETYPE_MOD) {
                                    auto filePathRel = fileEntry.path().lexically_relative(fs::current_path());
                                    auto filename = filePathRel.string();
                                    auto file = std::make_unique<FileData>();
                                    file->filename = filename;
                                    std::ifstream file_stream(filename);
                                    
                                    if (!file_stream) {
                                        LOG_ERROR("Could not open file &1", filename)
                                        continue;
                                    }

                                    try {
                                        file->j = json::parse(file_stream);
                                    } catch (json::parse_error& e) {
                                        std::stringstream ss;
                                        ss  << "File " << filename << " could not be parsed. Error at " << e.byte << ":"
                                            << e.what() << "\n";
                                        LOG_ERROR(ss.str())
                                        continue;
                                    }

                                    if (file->j.contains(MOD_HEADER_ENTRY)) {
                                        auto& header = file->j[MOD_HEADER_ENTRY];
                                        mod->name = retrieveTextFromJson(header, MOD_HEADER_NAME);
                                        mod->desc = retrieveTextFromJson(header, MOD_HEADER_DESC);
                                    }

                                    mod->files.push_back(std::move(file));
                                }
                            }

                            if (!mod->files.empty()) {
                                mods.push_back(std::move(mod));
                            }
                        }
                    }
                } catch (const fs::filesystem_error& e) {
                    LOG_ERROR("Filesystem error: &1", e.what())
                }
            }

            UText retrieveTextFromJson(json& header, std::string key) {
                if (!header.contains(key)) return nullptr;
                auto& json_key = header[key];
                if (json_key.is_string()) {
                    std::string result = json_key;
                    return std::make_unique<Text>(MOD_HEADER_TEXT_DEFAULT_LANGUAGE, result);
                } else {
                    auto text = std::make_unique<Text>();
                    text->from_json(json_key);
                    return std::move(text);
                }
            }

            void printModList() {
                std::cout << "List of mods:\n";
                if (mods.empty()) {
                    std::cout << "No mods\n";
                } else {
                    for(auto& mod : mods) {
                        std::cout << " - Name: " << mod->name->get() << "\n"
                            << "   Default name: " << mod->defaultname << "\n"
                            << "   Desc: " << mod->desc->get() << "\n"
                            << "   Path: " << mod->path << "\n"
                            << "   Files: " << mod->files.size() << "\n";
                        
                        for(auto& file : mod->files) {
                            std::cout << "    - File name: " << file->filename << "\n"
                                << "      json: " << file->j.dump(2) << "\n\n";
                        }
                        std::cout << "\n";
                    }
                }
            }

            AssetManager* getAssetManager() { return assetManager.get(); }

    };    
    
}
