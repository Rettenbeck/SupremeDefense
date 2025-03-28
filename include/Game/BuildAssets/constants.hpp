#include <Util/basic.hpp>
#include <ECS/constants.hpp>

#pragma once


namespace SupDef {
    
    const int DEFAULT_MAP_TILESIZE = 32;
    const int DEFAULT_MAP_WIDTH  = 640;
    const int DEFAULT_MAP_HEIGHT = 480;



    // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= //
    // =-=-=-=-=-=-=-=-=-=-=-=-= MAP DATA                                =-=-=-=-=-=-=-=-=-=-=-=-= //
    const std::string AS_MAP_EX                             = "example_map"                         ;
    const std::string AS_MAP_EX_NAME_DE                     = "Beispielkarte"                       ;
    const std::string AS_MAP_EX_DESC_DE                     = "Nur eine beispielhafte Karte"        ;



    // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= //
    // =-=-=-=-=-=-=-=-=-=-=-=-= PLAYER DATA                             =-=-=-=-=-=-=-=-=-=-=-=-= //
    const std::string AS_PLAYER_EX                          = "player"                              ;
    const std::string AS_PLAYER_EX_NAME_DE                  = "Spieler"                             ;
    const std::string AS_PLAYER_EX_DESC_DE                  = "Spieler"                             ;



    // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= //
    // =-=-=-=-=-=-=-=-=-=-=-=-= RESOURCES                               =-=-=-=-=-=-=-=-=-=-=-=-= //
    const std::string AS_RES_GOLD                           = "gold"                                ;
    const std::string AS_RES_GOLD_NAME_DE                   = "Gold"                                ;
    const std::string AS_RES_GOLD_DESC_DE                   = "Wertvolles Gold"                     ;

    const std::string AS_RES_WOOD                           = "wood"                                ;
    const std::string AS_RES_WOOD_NAME_DE                   = "Holz"                                ;
    const std::string AS_RES_WOOD_DESC_DE                   = "Gehacktes Holz"                      ;



    // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= //
    // =-=-=-=-=-=-=-=-=-=-=-=-= Commands                                =-=-=-=-=-=-=-=-=-=-=-=-= //
    const std::string AS_COM_MOVE                           = "com_move"                            ;
    const std::string AS_COM_MOVE_NAME_DE                   = "Bewegen"                             ;
    const std::string AS_COM_MOVE_DESC_DE                   = "Einheit bewegen"                     ;
    const std::string AS_COM_TECH_MOVE                      = "tech_move"                           ;
    
    const std::string AS_COM_TEST1                          = "com_test1"                           ;
    const std::string AS_COM_TEST1_NAME_DE                  = "Testbefehl 1"                        ;
    const std::string AS_COM_TEST1_DESC_DE                  = "Einfach was testen"                  ;
    const std::string AS_COM_TECH_TEST1                     = "com_tech_test1"                      ;
    
    const std::string AS_COM_TEST2                          = "com_test2"                           ;
    const std::string AS_COM_TEST2_NAME_DE                  = "Testbefehl 2"                        ;
    const std::string AS_COM_TEST2_DESC_DE                  = "Einfach was nochmals testen"         ;
    const std::string AS_COM_TECH_TEST2                     = "com_tech_test2"                      ;
    



    // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= //
    // =-=-=-=-=-=-=-=-=-=-=-=-= Movable Units                           =-=-=-=-=-=-=-=-=-=-=-=-= //
    
    // =-=-=-=-=-=-=-=-=-= Mover 1                                             =-=-=-=-=-=-=-=-=-= //
    const std::string AS_UNIT_MOVER_1                       = "mover1"                              ;
    const std::string AS_UNIT_MOVER_1_NAME_DE               = "Mover One"                           ;
    const std::string AS_UNIT_MOVER_1_DESC_DE               = "Einfache Einheit zum Testen"         ;
    const float AS_UNIT_MOVER_1_SPEED               = 2.0;
    const float AS_UNIT_MOVER_1_DUMMY_RADIUS        = 6.0;
    const bool  AS_UNIT_MOVER_1_GROUND_BASED        = true;

    // =-=-=-=-=-=-=-=-=-= Mover 2                                             =-=-=-=-=-=-=-=-=-= //
    const std::string AS_UNIT_MOVER_2                       = "mover2"                              ;
    const std::string AS_UNIT_MOVER_2_NAME_DE               = "Mover Two"                           ;
    const std::string AS_UNIT_MOVER_2_DESC_DE               = "Etwas schneller als M1"              ;
    const float AS_UNIT_MOVER_2_SPEED               = 3.2;
    const float AS_UNIT_MOVER_2_DUMMY_RADIUS        = 8.0;
    const bool  AS_UNIT_MOVER_2_GROUND_BASED        = true;

    



    // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= //
    // =-=-=-=-=-=-=-=-=-=-=-=-= Immovable Units                         =-=-=-=-=-=-=-=-=-=-=-=-= //
    
    // =-=-=-=-=-=-=-=-=-= Immovable 1                                         =-=-=-=-=-=-=-=-=-= //
    const std::string AS_UNIT_IMM_1                         = "imm1"                                ;
    const std::string AS_UNIT_IMM_1_NAME_DE                 = "Gebäude 1"                           ;
    const std::string AS_UNIT_IMM_1_DESC_DE                 = "Testgebäude 1"                       ;
    const float AS_UNIT_IMM_1_WIDTH                 = 32.0;
    const float AS_UNIT_IMM_1_HEIGHT                = 32.0;
    const bool AS_UNIT_IMM_1_TILES_CHECK            = true;
    const bool AS_UNIT_IMM_1_COL_CHECK              = false;
    const bool AS_UNIT_IMM_1_OCCUPY                 = true;
    const bool AS_UNIT_IMM_1_IMPASSABLE             = true;

    // =-=-=-=-=-=-=-=-=-= Immovable 2                                         =-=-=-=-=-=-=-=-=-= //
    const std::string AS_UNIT_IMM_2                         = "imm2"                                ;
    const std::string AS_UNIT_IMM_2_NAME_DE                 = "Gebäude 2"                           ;
    const std::string AS_UNIT_IMM_2_DESC_DE                 = "Testgebäude 2"                       ;
    const float AS_UNIT_IMM_2_WIDTH                 = 48.0;
    const float AS_UNIT_IMM_2_HEIGHT                = 32.0;
    const bool AS_UNIT_IMM_2_TILES_CHECK            = true;
    const bool AS_UNIT_IMM_2_COL_CHECK              = false;
    const bool AS_UNIT_IMM_2_OCCUPY                 = true;
    const bool AS_UNIT_IMM_2_IMPASSABLE             = true;

}
