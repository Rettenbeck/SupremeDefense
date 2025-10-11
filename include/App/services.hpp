#pragma once

#include <App/settings.hpp>
#include <App/Services/asset_service.hpp>
#include <App/Services/file_manager.hpp>


namespace SupDef {

    struct Services {
        USettings settings;
        UEventDispatcher globalDispatcher;
        UAssetService assetService;
        UFileManager fileManager;

        Services() {
            settings = std::make_unique<Settings>();
            globalDispatcher = std::make_unique<EventDispatcher>();
            assetService = std::make_unique<AssetService>(globalDispatcher.get());
            fileManager = std::make_unique<FileManager>(globalDispatcher.get());

            assetService->setSettings(settings.get());
            assetService->initialize();
        }
    };

    DEFINE_UNIQUE(Services, UServices)

}
