#pragma once

#include <Util/basic.hpp>
#include <ECS/constants.hpp>


namespace SupDef {
    
    const int DEFAULT_MAP_TILESIZE = 32;
    const int DEFAULT_MAP_WIDTH  = 640;
    const int DEFAULT_MAP_HEIGHT = 480;

    struct WaveDataSingle {
        long waveNumber, frameCount; float x, y; AssetID enemyID;
    };
    using WaveData = std::vector<WaveDataSingle>;



    // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= //
    // =-=-=-=-=-=-=-=-=-=-=-=-= World DATA                              =-=-=-=-=-=-=-=-=-=-=-=-= //
    const std::string AS_WORLD_WINTER_MAUL                  = "world_winter_maul"                   ;



    // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= //
    // =-=-=-=-=-=-=-=-=-=-=-=-= MAP DATA                                =-=-=-=-=-=-=-=-=-=-=-=-= //
    const std::string AS_MAP_EX                             = "map_example"                         ;
    const std::string AS_MAP_EX_NAME_DE                     = "Beispielkarte"                       ;
    const std::string AS_MAP_EX_DESC_DE                     = "Nur eine beispielhafte Karte"        ;

    const std::string AS_MAP_WINTER_MAUL                    = "map_winter_maul"                     ;
    const std::string AS_MAP_WINTER_MAUL_NAME_DE            = "Winter Maul"                         ;
    const std::string AS_MAP_WINTER_MAUL_DESC_DE            = "Winter Maul ist zurück!"             ;
    const int AS_MAP_WINTER_MAUL_WIDTH                      = 2000;
    const int AS_MAP_WINTER_MAUL_HEIGHT                     = 1600;

    VF2s AS_MAP_WINTER_MAUL_PLAYER_SPAWNS = {
        {  50.0f,   50.0f},
        { 620.0f,   50.0f},
        { 620.0f,  620.0f}
    };

    VF2s AS_MAP_WINTER_MAUL_ENEMY_SPAWNS = {
        {  80.0f, 1250.0f},
        { 420.0f, 1250.0f}
    };

    VF2s AS_MAP_WINTER_MAUL_ENEMY_GOALS = {
        { 450.0f,  150.0f},
        { 750.0f,  150.0f}
    };

    VF2s AS_MAP_WINTER_MAUL_WINTER_WALLS = {
        { 256.0f,   64.0f},
        { 256.0f,  128.0f},
        { 256.0f,  192.0f}
    };




    // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= //
    // =-=-=-=-=-=-=-=-=-=-=-=-= WALL DATA                               =-=-=-=-=-=-=-=-=-=-=-=-= //
    const std::string AS_WALL_WINTER                        = "wall_winter"                         ;
    const std::string AS_WALL_WINTER_NAME_DE                = "Wand"                                ;
    const std::string AS_WALL_WINTER_DESC_DE                = "Verschneite Wand"                    ;
    const float AS_WALL_WINTER_WIDTH                        = 64.0;
    const float AS_WALL_WINTER_HEIGHT                       = 64.0;
    const bool AS_WALL_WINTER_TILES_CHECK                   = true;
    const bool AS_WALL_WINTER_COL_CHECK                     = false;
    const bool AS_WALL_WINTER_OCCUPY                        = true;
    const bool AS_WALL_WINTER_IMPASSABLE                    = true;



    // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= //
    // =-=-=-=-=-=-=-=-=-=-=-=-= PLAYER ROLE DATA                        =-=-=-=-=-=-=-=-=-=-=-=-= //
    const std::string AS_PLAYER_ROLE_ALL                    = "player_role_all"                     ;
    const std::string AS_PLAYER_ROLE_ALL_NAME_DE            = "Keine Einschränkungen"               ;
    const std::string AS_PLAYER_ROLE_ALL_DESC_DE            = "Auswahl ohne Einschränkungen"        ;



    // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= //
    // =-=-=-=-=-=-=-=-=-=-=-=-= PLAYER DATA                             =-=-=-=-=-=-=-=-=-=-=-=-= //
    const std::string AS_PLAYER_EX                          = "player"                              ;
    const std::string AS_PLAYER_EX_NAME_DE                  = "Spieler"                             ;
    const std::string AS_PLAYER_EX_DESC_DE                  = "Spieler"                             ;
    const std::string AS_PLAYER_EX_STARTER                  = "spider"                              ;

    const std::string AS_PLAYER_EX_NO_MOVE                  = "player_no_move"                      ;
    const std::string AS_PLAYER_EX_NO_MOVE_NAME_DE          = "Spieler (nm)"                        ;
    const std::string AS_PLAYER_EX_NO_MOVE_DESC_DE          = "Spieler (nm)"                        ;
    const std::string AS_PLAYER_EX_NO_MOVE_STARTER          = "imm1"                                ;

    const std::string AS_PLAYER_HUMAN                       = "player_human"                        ;
    const std::string AS_PLAYER_HUMAN_NAME_DE               = "Menschen"                            ;
    const std::string AS_PLAYER_HUMAN_DESC_DE               = "Bauen durchschnittliche Türme."      ;
    const std::string AS_PLAYER_HUMAN_STARTER               = "imm_human_main"                      ;



    // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= //
    // =-=-=-=-=-=-=-=-=-=-=-=-= RESOURCES                               =-=-=-=-=-=-=-=-=-=-=-=-= //
    const std::string AS_RES_GOLD                           = "gold"                                ;
    const std::string AS_RES_GOLD_NAME_DE                   = "Gold"                                ;
    const std::string AS_RES_GOLD_DESC_DE                   = "Wertvolles Gold"                     ;

    const std::string AS_RES_WOOD                           = "wood"                                ;
    const std::string AS_RES_WOOD_NAME_DE                   = "Holz"                                ;
    const std::string AS_RES_WOOD_DESC_DE                   = "Gehacktes Holz"                      ;



    // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= //
    // =-=-=-=-=-=-=-=-=-=-=-=-= TECHS                                   =-=-=-=-=-=-=-=-=-=-=-=-= //
    const std::string AS_TECH_ALLOW_BUILD1                  = "tech_allow_build_indirect1"          ;
    const std::string AS_TECH_ALLOW_BUILD1_NAME_DE          = "Indirekt bauen"                      ;
    const std::string AS_TECH_ALLOW_BUILD1_DESC_DE          = "Bauen über selbst"                   ;
    const float AS_TECH_ALLOW_BUILD1_RADIUS                 = 90.0;



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
    const float AS_COM_TEST2_RADIUS                         = 70.0;
    
    const std::string AS_COM_BUILD_ARCHER_TOWER             = "com_build_archer_tower"              ;
    const std::string AS_COM_BUILD_ARCHER_TOWER_NAME_DE     = "Bogenschützenturm bauen"             ;
    const std::string AS_COM_TECH_BUILD_ARCHER_TOWER        = "tech_build_archer_tower"              ;
    


    // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= //
    // =-=-=-=-=-=-=-=-=-=-=-=-= Movable Units                           =-=-=-=-=-=-=-=-=-=-=-=-= //
    
    // =-=-=-=-=-=-=-=-=-= Mover 1                                             =-=-=-=-=-=-=-=-=-= //
    const std::string AS_UNIT_MOVER_1                       = "mover1"                              ;
    const std::string AS_UNIT_MOVER_1_NAME_DE               = "Mover One"                           ;
    const std::string AS_UNIT_MOVER_1_DESC_DE               = "Einfache Einheit zum Testen"         ;
    const long AS_UNIT_MOVER_1_HEALTH                       = 300;
    const float AS_UNIT_MOVER_1_SPEED                       = 3.0;
    const float AS_UNIT_MOVER_1_DUMMY_RADIUS                = 12.0;
    const bool  AS_UNIT_MOVER_1_GROUND_BASED                = true;

    // =-=-=-=-=-=-=-=-=-= Mover 2                                             =-=-=-=-=-=-=-=-=-= //
    const std::string AS_UNIT_MOVER_2                       = "mover2"                              ;
    const std::string AS_UNIT_MOVER_2_NAME_DE               = "Mover Two"                           ;
    const std::string AS_UNIT_MOVER_2_DESC_DE               = "Etwas groesser als M1"               ;
    const long AS_UNIT_MOVER_2_HEALTH                       = 400;
    const float AS_UNIT_MOVER_2_SPEED                       = 3.2;
    const float AS_UNIT_MOVER_2_DUMMY_RADIUS                = 15.0;
    const bool  AS_UNIT_MOVER_2_GROUND_BASED                = true;

    // =-=-=-=-=-=-=-=-=-= Spider                                              =-=-=-=-=-=-=-=-=-= //
    const std::string AS_UNIT_SPIDER                        = "spider"                              ;
    const std::string AS_UNIT_SPIDER_NAME_DE                = "Spinne"                              ;
    const std::string AS_UNIT_SPIDER_DESC_DE                = "Einfache Spinne"                     ;
    const long AS_UNIT_SPIDER_HEALTH                        = 500;
    const float AS_UNIT_SPIDER_SPEED                        = 1.8;
    const float AS_UNIT_SPIDER_DUMMY_RADIUS                 = 20.0;
    const bool  AS_UNIT_SPIDER_GROUND_BASED                 = true;



    // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= //
    // =-=-=-=-=-=-=-=-=-=-=-=-= Immovable Units                         =-=-=-=-=-=-=-=-=-=-=-=-= //
    
    // =-=-=-=-=-=-=-=-=-= Immovable 1                                         =-=-=-=-=-=-=-=-=-= //
    const std::string AS_UNIT_IMM_1                         = "imm1"                                ;
    const std::string AS_UNIT_IMM_1_NAME_DE                 = "Gebäude 1"                           ;
    const std::string AS_UNIT_IMM_1_DESC_DE                 = "Testgebäude 1"                       ;
    const float AS_UNIT_IMM_1_WIDTH                         = 32.0;
    const float AS_UNIT_IMM_1_HEIGHT                        = 32.0;
    const bool AS_UNIT_IMM_1_TILES_CHECK                    = true;
    const bool AS_UNIT_IMM_1_COL_CHECK                      = false;
    const bool AS_UNIT_IMM_1_OCCUPY                         = true;
    const bool AS_UNIT_IMM_1_IMPASSABLE                     = true;

    // =-=-=-=-=-=-=-=-=-= Immovable 2                                         =-=-=-=-=-=-=-=-=-= //
    const std::string AS_UNIT_IMM_2                         = "imm2"                                ;
    const std::string AS_UNIT_IMM_2_NAME_DE                 = "Gebäude 2"                           ;
    const std::string AS_UNIT_IMM_2_DESC_DE                 = "Testgebäude 2"                       ;
    const float AS_UNIT_IMM_2_WIDTH                         = 48.0;
    const float AS_UNIT_IMM_2_HEIGHT                        = 32.0;
    const bool AS_UNIT_IMM_2_TILES_CHECK                    = true;
    const bool AS_UNIT_IMM_2_COL_CHECK                      = false;
    const bool AS_UNIT_IMM_2_OCCUPY                         = true;
    const bool AS_UNIT_IMM_2_IMPASSABLE                     = true;

    // =-=-=-=-=-=-=-=-=-= Human main building                                 =-=-=-=-=-=-=-=-=-= //
    const std::string AS_UNIT_HUMAN_MAIN                    = "imm_human_main"                      ;
    const std::string AS_UNIT_HUMAN_MAIN_NAME_DE            = "Menschen Hauptgebäude"               ;
    const std::string AS_UNIT_HUMAN_MAIN_DESC_DE            = "Von hier aus kann man Türme der Menschen bauen.";
    const float AS_UNIT_HUMAN_MAIN_WIDTH                    = 64.0;
    const float AS_UNIT_HUMAN_MAIN_HEIGHT                   = 64.0;

    // =-=-=-=-=-=-=-=-=-= Human archer tower                                  =-=-=-=-=-=-=-=-=-= //
    const std::string AS_UNIT_ARCHER_TOWER                  = "imm_archer_tower"                    ;
    const std::string AS_UNIT_ARCHER_TOWER_NAME_DE          = "Bogenschützenturm"                   ;
    const std::string AS_UNIT_ARCHER_TOWER_DESC_DE          = "Einfacher Turm zur Gegnerabwehr."    ;
    const float AS_UNIT_ARCHER_TOWER_WIDTH                  = 64.0;
    const float AS_UNIT_ARCHER_TOWER_HEIGHT                 = 64.0;

    // =-=-=-=-=-=-=-=-=-= Human fire tower                                    =-=-=-=-=-=-=-=-=-= //
    const std::string AS_UNIT_FIRE_TOWER                    = "imm_fire_tower"                      ;
    const std::string AS_UNIT_FIRE_TOWER_NAME_DE            = "Feuerturm"                           ;
    const std::string AS_UNIT_FIRE_TOWER_DESC_DE            = "Kann Gegner in Brand setzen."        ;
    const float AS_UNIT_FIRE_TOWER_WIDTH                    = 64.0;
    const float AS_UNIT_FIRE_TOWER_HEIGHT                   = 64.0;

    // =-=-=-=-=-=-=-=-=-= Human wizard tower                                  =-=-=-=-=-=-=-=-=-= //
    const std::string AS_UNIT_WIZ_TOWER                     = "imm_wiz_tower"                       ;
    const std::string AS_UNIT_WIZ_TOWER_NAME_DE             = "Zaubererturm"                        ;
    const std::string AS_UNIT_WIZ_TOWER_DESC_DE             = "Verzaubert Gegner."                  ;
    const float AS_UNIT_WIZ_TOWER_WIDTH                     = 64.0;
    const float AS_UNIT_WIZ_TOWER_HEIGHT                    = 64.0;



    // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= //
    // =-=-=-=-=-=-=-=-=-=-=-=-= Weapons                                 =-=-=-=-=-=-=-=-=-=-=-=-= //
    
    // =-=-=-=-=-=-=-=-=-= Weapon Basic                                        =-=-=-=-=-=-=-=-=-= //
    const std::string AS_WEAPON_BASIC                       = "weapon_basic"                        ;
    const std::string AS_WEAPON_BASIC_NAME_DE               = "Standard-Waffe"                      ;
    const std::string AS_WEAPON_BASIC_DESC_DE               = "Einfache Waffe ohne Verfolgung"      ;
    const long AS_WEAPON_BASIC_DAMAGE                       = 12;
    const long AS_WEAPON_BASIC_PIERCE                       = 2;
    const DamageType AS_WEAPON_BASIC_DTYPE                  = DamageType::Normal;
    const int AS_WEAPON_BASIC_COOLDOWN                      = 4500;
    const float AS_WEAPON_BASIC_RADIUS                      = 100.0;
    
    // =-=-=-=-=-=-=-=-=-= Weapon Basic Homer                                  =-=-=-=-=-=-=-=-=-= //
    const std::string AS_WEAPON_BASIC_HOMER                 = "weapon_basic_homing"                 ;
    const std::string AS_WEAPON_BASIC_HOMER_NAME_DE         = "Verfolgende Waffe"                   ;
    const std::string AS_WEAPON_BASIC_HOMER_DESC_DE         = "Einfache Waffe mit Verfolgung"       ;
    const long AS_WEAPON_BASIC_HOMER_DAMAGE                 = 8;
    const long AS_WEAPON_BASIC_HOMER_PIERCE                 = 1;
    const DamageType AS_WEAPON_BASIC_HOMER_DTYPE            = DamageType::Normal;
    const int AS_WEAPON_BASIC_HOMER_COOLDOWN                = 3600;
    const float AS_WEAPON_BASIC_HOMER_RADIUS                = 130.0;
    
    // =-=-=-=-=-=-=-=-=-= Weapon Archer                                       =-=-=-=-=-=-=-=-=-= //
    const std::string AS_WEAPON_ARCHER                      = "weapon_archer"                       ;
    const std::string AS_WEAPON_ARCHER_NAME_DE              = "Pfeilschuss"                         ;
    const std::string AS_WEAPON_ARCHER_DESC_DE              = "Simpler Schuss, der durch Gegner schlägt.";
    const long AS_WEAPON_ARCHER_DAMAGE                      = 12;
    const long AS_WEAPON_ARCHER_PIERCE                      = 3;
    const DamageType AS_WEAPON_ARCHER_DTYPE                 = DamageType::Normal;
    const int AS_WEAPON_ARCHER_COOLDOWN                     = 4500;
    const float AS_WEAPON_ARCHER_RADIUS                     = 100.0;
    
    // =-=-=-=-=-=-=-=-=-= Weapon Fire                                         =-=-=-=-=-=-=-=-=-= //
    const std::string AS_WEAPON_FIRE                        = "weapon_fire"                         ;
    const std::string AS_WEAPON_FIRE_NAME_DE                = "Feuerschuss"                         ;
    const std::string AS_WEAPON_FIRE_DESC_DE                = "Verbrennt Gegner."                   ;
    const long AS_WEAPON_FIRE_DAMAGE                        = 8;
    const long AS_WEAPON_FIRE_PIERCE                        = 1;
    const DamageType AS_WEAPON_FIRE_DTYPE                   = DamageType::Fire;
    const int AS_WEAPON_FIRE_COOLDOWN                       = 5500;
    const float AS_WEAPON_FIRE_RADIUS                       = 120.0;
    
    // =-=-=-=-=-=-=-=-=-= Weapon Fire                                         =-=-=-=-=-=-=-=-=-= //
    const std::string AS_WEAPON_WIZ                         = "weapon_wiz"                          ;
    const std::string AS_WEAPON_WIZ_NAME_DE                 = "Zauberschuss"                        ;
    const std::string AS_WEAPON_WIZ_DESC_DE                 = "Hohe Angriffsgeschwindigkeit."       ;
    const long AS_WEAPON_WIZ_DAMAGE                         = 9;
    const long AS_WEAPON_WIZ_PIERCE                         = 1;
    const DamageType AS_WEAPON_WIZ_DTYPE                    = DamageType::Normal;
    const int AS_WEAPON_WIZ_COOLDOWN                        = 1300;
    const float AS_WEAPON_WIZ_RADIUS                        = 140.0;



    // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= //
    // =-=-=-=-=-=-=-=-=-=-=-=-= Projectiles                             =-=-=-=-=-=-=-=-=-=-=-=-= //
    
    // =-=-=-=-=-=-=-=-=-= Projectile Basic                                    =-=-=-=-=-=-=-=-=-= //
    const std::string AS_PROJECTILE_BASIC                   = "proj_basic"                          ;
    const std::string AS_PROJECTILE_BASIC_NAME_DE           = "Standard-Projektil"                  ;
    const std::string AS_PROJECTILE_BASIC_DESC_DE           = "Einfaches Geschoss ohne Verfolgung"  ;
    const int AS_PROJECTILE_BASIC_MULTIHIT                  = 0;
    const bool AS_PROJECTILE_BASIC_HOMING                   = false;
    const float AS_PROJECTILE_BASIC_RADIUS                  = 4.0;
    const float AS_PROJECTILE_BASIC_SPEED                   = 10.0;
    
    // =-=-=-=-=-=-=-=-=-= Projectile Basic Homer                              =-=-=-=-=-=-=-=-=-= //
    const std::string AS_PROJECTILE_BASIC_HOMER             = "proj_basic_homer"                    ;
    const std::string AS_PROJECTILE_BASIC_HOMER_NAME_DE     = "Verfolgendes Projektil"              ;
    const std::string AS_PROJECTILE_BASIC_HOMER_DESC_DE     = "Einfaches Geschoss mit Verfolgung"   ;
    const int AS_PROJECTILE_BASIC_HOMER_MULTIHIT            = 0;
    const bool AS_PROJECTILE_BASIC_HOMER_HOMING             = true;
    const float AS_PROJECTILE_BASIC_HOMER_RADIUS            = 4.0;
    const float AS_PROJECTILE_BASIC_HOMER_SPEED             = 10.0;
    
    // =-=-=-=-=-=-=-=-=-= Projectile Archer                                   =-=-=-=-=-=-=-=-=-= //
    const std::string AS_PROJECTILE_ARCHER                  = "proj_archer"                         ;
    const std::string AS_PROJECTILE_ARCHER_NAME_DE          = "Pfeil"                               ;
    const int AS_PROJECTILE_ARCHER_MULTIHIT                 = 0;
    const bool AS_PROJECTILE_ARCHER_HOMING                  = false;
    const float AS_PROJECTILE_ARCHER_RADIUS                 = 8.0;
    const float AS_PROJECTILE_ARCHER_SPEED                  = 10.0;



    // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= //
    // =-=-=-=-=-=-=-=-=-=-=-=-= Enemy Waves                             =-=-=-=-=-=-=-=-=-=-=-=-= //
    
    // =-=-=-=-=-=-=-=-=-= Wave 1                                              =-=-=-=-=-=-=-=-=-= //
    const std::string AS_WAVES_SPIDER1                      = "waves_spider1"                       ;
    WaveData AS_WAVES_SPIDER1_DATA = {
        {1, 100, 0, 0, AS_UNIT_SPIDER},
        {1, 200, 0, 0, AS_UNIT_SPIDER},
        {1, 300, 0, 0, AS_UNIT_SPIDER},
        {1, 400, 0, 0, AS_UNIT_SPIDER},
        {1, 500, 0, 0, AS_UNIT_SPIDER},
        {1, 600, 0, 0, AS_UNIT_SPIDER}
    };
    
    // =-=-=-=-=-=-=-=-=-= Wave 2                                              =-=-=-=-=-=-=-=-=-= //
    const std::string AS_WAVES_SPIDER2                      = "waves_spider2"                       ;
    WaveData AS_WAVES_SPIDER2_DATA = {
        {1, 100,  0, 10, AS_UNIT_SPIDER},
        {1, 160,  0, 20, AS_UNIT_SPIDER},
        {1, 220,  0,  0, AS_UNIT_SPIDER},
        {1, 280, 10,  5, AS_UNIT_SPIDER},
        {1, 440, 20,  0, AS_UNIT_SPIDER},
        {1, 550,  0,  5, AS_UNIT_SPIDER}
    };
    


}
