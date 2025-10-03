#pragma once

#include <Util/basic.hpp>


namespace SupDef {
    
    using PageId = uint32_t;
    using PageTypeId = int;

    using GuiMemberFunc = std::function<void()>;  // Button <-> function map
    using TableLine = std::vector<std::string>;
    using TableData = std::vector<TableLine>;

    const PageTypeId PAGE_TYPE_ID_START             = 1;
    const PageTypeId PAGE_TYPE_ID_OPEN_SERVER       = 10;
    const PageTypeId PAGE_TYPE_ID_JOIN_SERVER       = 15;
    const PageTypeId PAGE_TYPE_ID_GAME              = 100;

    const int GUI_NW_INPUT_KEY_PLAYER_NAME = 1;
    const int GUI_NW_INPUT_KEY_PORT        = 2;
    const int GUI_NW_INPUT_KEY_IP          = 3;

}
