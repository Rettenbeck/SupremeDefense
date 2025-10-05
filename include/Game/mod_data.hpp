#pragma once

#include <Misc/text.hpp>
#include <nlohmann/include.hpp>


namespace SupDef {
    
    struct FileData {
        std::string filename = "";
        json j;
    };
    
    using UFile = std::unique_ptr<FileData>;
    using Files = std::vector<UFile>;

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
        // std::string getActive() {
        //     return (active ? "y" : "n");
        // }
    };
    
    using UMod = std::unique_ptr<ModData>;
    using Mods = std::vector<UMod>;

}
