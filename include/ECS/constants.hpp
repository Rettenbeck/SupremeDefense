#include <Util/basic.hpp>

#pragma once


namespace SupDef {
    
    // Defines
    using EntityID  = uint32_t;
    using EntityIDs = std::vector<EntityID>;
    using EntityStatus = uint32_t;

    using AssetID = std::string;
    using AssetIDs = std::vector<AssetID>;
    using CommandID = std::string;
    using ResourceID = std::string;

    using CommandType = std::string;
    
    using Cooldown = long;
    const Cooldown COOLDOWN_SEC = 12000;
    const Cooldown COOLDOWN_FRAME = COOLDOWN_SEC / 60;
    const Cooldown NO_COOLDOWN = -1;

    const EntityID NO_ENTITY = 0;
    const AssetID NO_ASSET = "_";
    const CommandID NO_COMMAND = "_";
    const std::string NO_NAME = "unknown asset";
    const std::string NO_DESC = "???";

    const CommandType JCOM_MESSAGE  = "message";
    const CommandType JCOM_UNIQUE   = "unique_command";
    const CommandType JCOM_PARENT   = "parent";
    const CommandType JCOM_X        = "x";
    const CommandType JCOM_Y        = "y";
    

    // Non-component objects
    const std::string SX_NAME                               = "name"                                ;
    const std::string SX_DESC                               = "desc"                                ;
    const std::string SX_LANG_DE                            = "de"                                  ;
    const std::string SX_LANG_EN                            = "en"                                  ;
    const std::string SX_RESOURCE                           = "resource"                            ;
    const std::string SX_AMOUNT                             = "amount"                              ;
    const std::string SX_RATE                               = "gather_rate"                         ;
    const std::string SX_MAX_CAP                            = "max_capacity"                        ;
    const std::string SX_SELF_COST                          = "self_cost"                           ;
    
    // Components
    const std::string SC_TEXT                               = "text"                                ;
    const std::string SC_PLAYER                             = "player"                              ;
    const std::string SC_TEAM                               = "team"                                ;
    const std::string SC_REQUIREMENT                        = "requirement"                         ;
    const std::string SC_RESOURCE                           = "resource"                            ;
    const std::string SC_PLAYER_OWNERSHIP                   = "player_ownership"                    ;
    const std::string SC_WORLD                              = "world"                               ;
    const std::string SC_MAP                                = "map"                                 ;
    const std::string SC_TILES                              = "tiles"                               ;
    const std::string SC_SELECTABLE                         = "selectable"                          ;
    const std::string SC_POSITION                           = "position"                            ;
    const std::string SC_MOVEMENT                           = "movement"                            ;
    const std::string SC_IMMOVABLE                          = "immovable"                           ;
    const std::string SC_COLLISION                          = "collision"                           ;
    const std::string SC_HEALTH                             = "health"                              ;

    // Asset Components
    const std::string SCA_ASSET                             = "asset"                               ;
    const std::string SCA_COMMAND                           = "command"                             ;
    const std::string SCA_BUILD_COMMAND                     = "build_command"                       ;
    const std::string SCA_INIT_CONTAINER                    = "init_container"                      ;
    const std::string SCA_RESOURCE_DEF                      = "resource_def"                        ;

    // Tech Components
    const std::string SCT_TECH                              = "tech"                                ;   
    const std::string SCT_ACTIVE_TECH                       = "active_tech"                         ;   

    // Attributes
    const std::string S_IS_UNIQUE                           = "unique"                              ;
    const std::string S_RESOURCES                           = "resources"                           ;
    const std::string S_TO_BUILD                            = "to_build"                            ;
    const std::string S_CONTAINED                           = "contained"                           ;
    const std::string S_COOLDOWN                            = "cooldown"                            ;
    const std::string S_CURRENT_COOLDOWN                    = "current_cooldown"                    ;
    const std::string S_INIT_FULL_COOLDOWN                  = "init_with_full_cooldown"             ;
    const std::string S_COMMAND                             = "command"                             ;

    const std::string S_ASSIGNEES                           = "assignees"                           ;

    const std::string S_KEY                                 = "key"                                 ;
    const std::string S_TEXTS                               = "texts"                               ;
    const std::string S_ID                                  = "id"                                  ;
    const std::string S_ASSET_ID                            = "asset_id"                            ;
    const std::string S_ENTITIES                            = "entities"                            ;
    const std::string S_ASSETS                              = "assets"                              ;
    const std::string S_COMPONENTS                          = "components"                          ;
    const std::string S_NEXT_ENTITY_ID                      = "next_entity_id"                      ;
    const std::string S_NAME                                = "name"                                ;
    const std::string S_TYPE                                = "type"                                ;
    const std::string S_DESCRIPTION                         = "description"                         ;
    const std::string S_RESOURCE                            = "resource"                            ;
    const std::string S_OWNER                               = "owner"                               ;
    const std::string S_ACCESSORS                           = "accessors"                           ;
    const std::string S_PARENT                              = "parent"                              ;
    const std::string S_CHILDREN                            = "children"                            ;
    const std::string S_PLAYER                              = "player"                              ;
    const std::string S_TEAM                                = "team"                                ;
    const std::string S_RULES                               = "rules"                               ;
    const std::string S_MAPS                                = "maps"                                ;
    const std::string S_TILES                               = "tile_list"                           ;
    const std::string S_TILE_SIZE                           = "tile_size"                           ;
    const std::string S_TILES_X                             = "tiles_x"                             ;
    const std::string S_TILES_Y                             = "tiles_y"                             ;
    const std::string S_IS_IMPASSABLE                       = "is_impassable"                       ;
    const std::string S_IS_OCCUPIED                         = "is_occupied"                         ;
    const std::string S_MOVEMENT_COST                       = "movement_cost"                       ;
    const std::string S_PLAYERS                             = "players"                             ;
    const std::string S_X                                   = "X"                                   ;
    const std::string S_Y                                   = "Y"                                   ;
    const std::string S_X_ABS                               = "X_absolute"                          ;
    const std::string S_Y_ABS                               = "Y_absolute"                          ;
    const std::string S_VX                                  = "VX"                                  ;
    const std::string S_VY                                  = "VY"                                  ;

    const std::string S_SPEED                               = "speed"                               ;
    const std::string S_ORIGINAL_SPEED                      = "original_speed"                      ;
    const std::string S_IS_GROUND_BASED                     = "is_ground_based"                     ;
    const std::string S_HAS_GOAL                            = "has_goal"                            ;
    const std::string S_GOAL_X                              = "goal_X"                              ;
    const std::string S_GOAL_Y                              = "goal_Y"                              ;
    const std::string S_TEMP_GOAL_X                         = "temp_goal_X"                         ;
    const std::string S_TEMP_GOAL_Y                         = "temp_goal_Y"                         ;

    const std::string S_REQUIRES_TILE_CHECK                 = "requires_tile_check"                 ;
    const std::string S_REQUIRES_COLLISION_CHECK            = "requires_collision_check"            ;
    const std::string S_MARKS_TILES_OCCUPIED                = "marks_tiles_occupied"                ;
    const std::string S_MARKS_TILES_IMPASSABLE              = "marks_tiles_impassable"              ;
    const std::string S_PLACED_DOWN                         = "placed_down"                         ;
    const std::string S_DUMMY_RADIUS                        = "dummy_radius"                        ;
    const std::string S_SHAPES                              = "shapes"                              ;
    const std::string S_OFFSET_X                            = "offsetX"                             ;
    const std::string S_OFFSET_Y                            = "offsetY"                             ;
    const std::string S_SHAPE_TYPE                          = "shape_type"                          ;
    const std::string S_SHAPE_TYPE_CIRCLE                   = "circle"                              ;
    const std::string S_RADIUS                              = "radius"                              ;
    const std::string S_SHAPE_TYPE_RECT                     = "rectangle"                           ;
    const std::string S_WIDTH                               = "width"                               ;
    const std::string S_HEIGHT                              = "height"                              ;
    const std::string S_HEALTH                              = "health"                              ;


}
