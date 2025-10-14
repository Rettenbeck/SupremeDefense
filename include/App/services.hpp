#pragma once

#include <App/settings.hpp>
#include <App/Services/asset_service.hpp>
#include <App/Services/file_manager.hpp>
#include <App/Services/data_checker.hpp>
#include <App/Services/game_starter.hpp>


namespace SupDef {

    struct Services {
        USettings settings;
        UEventDispatcher globalDispatcher;
        UAssetService assetService;
        UFileManager fileManager;
        UDataChecker dataChecker;
        UGameStarter gameStarter;

        Services() {
            settings = std::make_unique<Settings>();
            globalDispatcher = std::make_unique<EventDispatcher>();
            assetService = std::make_unique<AssetService>();
            fileManager = std::make_unique<FileManager>();
            dataChecker = std::make_unique<DataChecker>();
            gameStarter = std::make_unique<GameStarter>();

            assetService->setSettings(settings.get());
            assetService->initialize();
        }
    };

    DEFINE_UNIQUE(Services, UServices)

}
