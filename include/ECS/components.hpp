#pragma once

#include <ECS/components/player_component.hpp>
#include <ECS/components/team_component.hpp>
#include <ECS/components/resource_component.hpp>
#include <ECS/components/player_ownership_component.hpp>
#include <ECS/components/world_component.hpp>
#include <ECS/components/map_component.hpp>
#include <ECS/components/tiles_component.hpp>
#include <ECS/components/selectable_component.hpp>
#include <ECS/components/position_component.hpp>
#include <ECS/components/movement_component.hpp>
#include <ECS/components/collision_component.hpp>
#include <ECS/components/immovable_component.hpp>
#include <ECS/components/influence_component.hpp>
#include <ECS/components/influenceable_component.hpp>
#include <ECS/components/projectile_component.hpp>
#include <ECS/components/projectile_target_component.hpp>
#include <ECS/components/projectile_hittable_component.hpp>
#include <ECS/components/weapon_component.hpp>
#include <ECS/components/health_component.hpp>
#include <ECS/components/enemy_spawner_component.hpp>
#include <ECS/components/enemy_goal_component.hpp>
#include <ECS/components/enemy_goal_seeker_component.hpp>

#include <ECS/asset_components/text_component.hpp>
#include <ECS/asset_components/command_component.hpp>
#include <ECS/asset_components/move_command_component.hpp>
#include <ECS/asset_components/build_command_component.hpp>
#include <ECS/asset_components/requirement_component.hpp>
#include <ECS/asset_components/resource_def_component.hpp>
#include <ECS/asset_components/player_spawn_component.hpp>

#include <ECS/initial_components/init_container_component.hpp>
#include <ECS/initial_components/init_position_component.hpp>
#include <ECS/initial_components/player_role_component.hpp>
#include <ECS/initial_components/enemy_waves_component.hpp>

#include <ECS/tech_components/tech_component.hpp>
#include <ECS/tech_components/active_tech_component.hpp>
#include <ECS/tech_components/gift_tech_component.hpp>

#include <ECS/frontend_components/graphic_component.hpp>
#include <ECS/frontend_components/animation_component.hpp>
