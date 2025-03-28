#include <Util/basic.hpp>
#include <ECS/constants.hpp>

#pragma once


namespace SupDef {
    
    enum class Relation {
        Same, Allied, Opposing, Neutral, Undefined
    };

    using CommandType = std::string;
    using CollisionGroup = std::string;
    using TechMap = std::unordered_map<EntityID, EntityIDs>;

    using EntityIDsTriple = std::tuple<EntityIDs, EntityIDs, EntityIDs>;

    using _EntWorld = std::tuple<Entity*, WorldComponent*>;
    using _EntMapTiles = std::tuple<Entity*, MapComponent*, TilesComponent*>;
    using _EntPosMovCol  = std::tuple<Entity*, PositionComponent*, MovementComponent*, CollisionComponent*>;
    using _EntPosMovCols = std::vector<_EntPosMovCol>;
    using _EntPosCol  = std::tuple<Entity*, PositionComponent*, CollisionComponent*>;
    using _EntPosCols = std::vector<_EntPosCol>;
    using _Map_EntPosCols = std::unordered_map<EntityID, _EntPosCols>;
    using _EntTech  = std::tuple<Entity*, TechComponent*>;
    using _EntTechs = std::vector<_EntTech>;
    
    const CollisionGroup CG_INFLUENCE                       = "cg_influence"                        ;
    const CollisionGroup CG_PROJECTILE                      = "cg_projectile"                       ;

    const std::string DEFAULT_FILENAME_SAVEGAME             = "game_save.txt"                       ;

    const std::string SG_GAME                               = "game"                                ;
    const std::string SG_ENTITY_MANAGER                     = "entity_manager"                      ;
    const std::string SG_ASSET_MANAGER                      = "asset_manager"                       ;
    const std::string SG_TECH_MANAGER                       = "tech_manager"                        ;
    const std::string SG_COLLISION_TRACKER                  = "collision_tracker"                   ;
    
    const std::string SG_ENTITY_A                           = "entityA"                             ;
    const std::string SG_ENTITY_B                           = "entityB"                             ;
    const std::string SG_FRAME_COUNT                        = "frameCount"                          ;
    const std::string SG_COLLISION_GROUP                    = "collisionGroup"                      ;

}
