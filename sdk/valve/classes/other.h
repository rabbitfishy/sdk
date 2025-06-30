#pragma once
#include "../interface/interface.h"

#define tick_interval	( interfaces->global_vars->interval_per_tick )
#define time_to_ticks	( value )  ( static_cast<int>( 0.5f + static_cast<float>( value ) / tick_interval ) )
#define ticks_to_time	( value )  ( tick_interval * static_cast<float>( value ) )
#define round_to_ticks	( value )  ( tick_interval * time_to_ticks( value ) )

enum game_flags : int
{
	fl_on_ground				= (1 << 0),		// entity is at rest / on the ground
	fl_ducking					= (1 << 1),		// player is fully crouched/uncrouched
	fl_anim_ducking				= (1 << 2),		// player is in the process of crouching or uncrouching but could be in transition
	fl_water_jump				= (1 << 3),		// player is jumping out of water
	fl_on_train					= (1 << 4),		// player is controlling a train, so movement commands should be ignored on client during prediction
	fl_in_rain					= (1 << 5),		// entity is standing in rain
	fl_frozen					= (1 << 6),		// player is frozen for 3rd-person camera
	fl_at_controls				= (1 << 7),		// player can't move, but keeps key inputs for controlling another entity
	fl_client					= (1 << 8),		// entity is a client (player)
	fl_fake_client				= (1 << 9),		// entity is a fake client, simulated server side; don't send network messages to them
	fl_in_water					= (1 << 10),	// entity is in water
	fl_fly						= (1 << 11),
	fl_swim						= (1 << 12),
	fl_conveyor					= (1 << 13),
	fl_npc						= (1 << 14),
	fl_godmode					= (1 << 15),
	fl_no_target				= (1 << 16),
	fl_aim_target				= (1 << 17),
	fl_partial_ground			= (1 << 18),	// entity is standing on a place where not all corners are valid
	fl_static_prop				= (1 << 19),	// entity is a static property
	fl_graphed					= (1 << 20),
	fl_grenade					= (1 << 21),
	fl_step_movement			= (1 << 22),
	fl_dont_touch				= (1 << 23),
	fl_base_velocity			= (1 << 24),	// entity have applied base velocity this frame
	fl_world_brush				= (1 << 25),	// entity is not moveable/removeable brush (part of the world, but represented as an entity for transparency or something)
	fl_object					= (1 << 26),
	fl_kill_me					= (1 << 27),	// entity is marked for death and will be freed by the game
	fl_on_fire					= (1 << 28),
	fl_dissolving				= (1 << 29),
	fl_trans_ragdoll			= (1 << 30),	// entity is turning into client-side ragdoll
	fl_unblockable_by_player	= (1 << 31)
};

enum game_entity_flags : int
{
	efl_kill_me								= (1 << 0),
	efl_dormant								= (1 << 1),
	efl_noclip_active						= (1 << 2),
	efl_setting_up_bones					= (1 << 3),
	efl_keep_on_recreate_entities			= (1 << 4),
	efl_dirty_shadowupdate					= (1 << 5),
	efl_notify								= (1 << 6),
	efl_force_check_transmit				= (1 << 7),
	efl_bot_frozen							= (1 << 8),
	efl_server_only							= (1 << 9),
	efl_no_auto_edict_attach				= (1 << 10),
	efl_dirty_abs_transform					= (1 << 11),
	efl_dirty_abs_velocity					= (1 << 12),
	efl_dirty_abs_ang_velocity				= (1 << 13),
	efl_dirty_surrounding_collision_bounds	= (1 << 14),
	efl_dirty_spatial_partition				= (1 << 15),
	efl_has_player_child					= (1 << 16),
	efl_in_skybox							= (1 << 17),
	efl_use_partition_when_not_solid		= (1 << 18),
	efl_touching_fluid						= (1 << 19),
	efl_is_being_lifted_by_barnacle			= (1 << 20),
	efl_no_rotorwash_push					= (1 << 21),
	efl_no_think_function					= (1 << 22),
	efl_no_game_physics_simulation			= (1 << 23),
	efl_check_untouch						= (1 << 24),
	efl_dont_block_los						= (1 << 25),
	efl_dont_walk_on						= (1 << 26),
	efl_no_dissolve							= (1 << 27),
	efl_no_megaphyscannon_ragdoll			= (1 << 28),
	efl_no_water_velocity_change			= (1 << 29),
	efl_no_physcannon_interaction			= (1 << 30),
	efl_no_damage_forces					= (1 << 31)
};

enum game_move_type : int
{
	move_type_none = 0,
	move_type_isometric,
	move_type_walk,
	move_type_step,
	move_type_fly,						// no gravity, but still collides with stuff
	move_type_fly_gravity,				// flies through the air and is affected by gravity
	move_type_vphysics,
	move_type_push,						// no clip to world, push and crush
	move_type_noclip,					// no gravity, no collisions, still do velocity/absvelocity
	move_type_ladder,
	move_type_observer,					// observer movement, depends on player's observer mode
	move_type_custom,
	move_type_last = move_type_custom,
	move_type_max_bits = 4
};

enum game_effect_flags : int
{
	ef_bonemerge					= (1 << 0),  // performs bone merge on client side
	ef_bright_light					= (1 << 1),  // dlight centered at entity origin
	ef_dim_light					= (1 << 2),  // player flashlight
	ef_no_interp					= (1 << 3),  // don't interpolate the next frame
	ef_no_shadow					= (1 << 4),  // don't cast no shadow
	ef_no_draw						= (1 << 5),  // don't draw entity
	ef_no_receive_shadow			= (1 << 6),  // don't receive no shadow
	ef_bonemerge_fastcull			= (1 << 7),  // for use with 'ef_bonemerge'. if this is set, then it places this ent's origin at its parent and uses the parent's bbox + the max extents of the aiment. otherwise, it sets up the parent's bones every frame to figure out where to place the aiment, which is inefficient because it'll setup the parent's bones even if the parent is not in the pvs
	ef_item_blink					= (1 << 8),  // blink an item so that the user notices it
	ef_parent_animates				= (1 << 9),  // always assume that the parent entity is animating
	ef_marked_for_fast_reflection	= (1 << 10), // marks an entity for reflection rendering when using "$reflectonlymarkedentities" material variable
	ef_no_shadow_depth				= (1 << 11), // indicates this entity does not render into any shadow depthmap
	ef_shadow_depth_nocache			= (1 << 12), // indicates this entity cannot be cached in shadow depthmap and should render every frame
	ef_no_flash_light				= (1 << 13),
	ef_no_csm						= (1 << 14), // indicates this entity does not render into the cascade shadow depthmap
	ef_max_bits						= 15
};

enum game_team_id : int
{
	team_unassigned = 0,
	team_spectator,
	team_t,
	team_ct
};

enum game_observer_mode : int
{
	obs_mode_none = 0,
	obs_mode_deathcam,
	obs_mode_freezecam,
	obs_mode_fixed,
	obs_mode_in_eye,
	obs_mode_chase,
	obs_mode_roaming
};