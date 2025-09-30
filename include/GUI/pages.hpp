#pragma once

#include <GUI/Pages/start_page.hpp>
#include <GUI/Pages/open_server_page.hpp>
#include <GUI/Pages/join_server_page.hpp>
#include <GUI/Pages/game_page.hpp>


namespace SupDef {

    void registerPages() {
        Page::addToRegistry<StartPage>(PAGE_ID_START);
        Page::addToRegistry<OpenServerPage>(PAGE_ID_OPEN_SERVER);
        Page::addToRegistry<JoinServerPage>(PAGE_ID_JOIN_SERVER);
        Page::addToRegistry<GamePage>(PAGE_ID_GAME);
    }

}
