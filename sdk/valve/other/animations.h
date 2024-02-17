#pragma once
#include "../maths/vector.h"
#include "../maths/q_angle.h"
#include "../maths/matrix.h"
#include "../maths/utl_vector.h"
#include "studio.h"
#include <assert.h>

#define max_animation_layer 13

#pragma region animations flags

enum game_animation_state_layer : int
{
	ANIMATION_LAYER_AIM_MATRIX = 0,
	ANIMATION_LAYER_WEAPON_ACTION,
	ANIMATION_LAYER_WEAPON_ACTION_RECROUCH,
	ANIMATION_LAYER_ADJUST,
	ANIMATION_LAYER_MOVEMENT_JUMP_OR_FALL,
	ANIMATION_LAYER_MOVEMENT_LAND_OR_CLIMB,
	ANIMATION_LAYER_MOVEMENT_MOVE,
	ANIMATION_LAYER_MOVEMENT_STRAFE_CHANGE,
	ANIMATION_LAYER_WHOLE_BODY,
	ANIMATION_LAYER_FLASHED,
	ANIMATION_LAYER_FLINCH,
	ANIMATION_LAYER_ALIVE_LOOP,
	ANIMATION_LAYER_LEAN,
	ANIMATION_LAYER_COUNT = 13
};

enum game_animation_pose_parameters : int
{
	PLAYER_POSE_PARAM_FIRST = 0,
	PLAYER_POSE_PARAM_LEAN_YAW = PLAYER_POSE_PARAM_FIRST,
	PLAYER_POSE_PARAM_SPEED,
	PLAYER_POSE_PARAM_LADDER_SPEED,
	PLAYER_POSE_PARAM_LADDER_YAW,
	PLAYER_POSE_PARAM_MOVE_YAW,
	PLAYER_POSE_PARAM_RUN,
	PLAYER_POSE_PARAM_BODY_YAW,
	PLAYER_POSE_PARAM_BODY_PITCH,
	PLAYER_POSE_PARAM_DEATH_YAW,
	PLAYER_POSE_PARAM_STAND,
	PLAYER_POSE_PARAM_JUMP_FALL,
	PLAYER_POSE_PARAM_AIM_BLEND_STAND_IDLE,
	PLAYER_POSE_PARAM_AIM_BLEND_CROUCH_IDLE,
	PLAYER_POSE_PARAM_STRAFE_DIR,
	PLAYER_POSE_PARAM_AIM_BLEND_STAND_WALK,
	PLAYER_POSE_PARAM_AIM_BLEND_STAND_RUN,
	PLAYER_POSE_PARAM_AIM_BLEND_CROUCH_WALK,
	PLAYER_POSE_PARAM_MOVE_BLEND_WALK,
	PLAYER_POSE_PARAM_MOVE_BLEND_RUN,
	PLAYER_POSE_PARAM_MOVE_BLEND_CROUCH_WALK,
	//PLAYER_POSE_PARAM_STRAFE_CROSS,
	PLAYER_POSE_PARAM_COUNT
};

enum game_sequence_activity : int
{
	ACT_INVALID = -1,
	ACT_CSGO_NULL = 957,
	ACT_CSGO_DEFUSE,
	ACT_CSGO_DEFUSE_WITH_KIT,
	ACT_CSGO_FLASHBANG_REACTION,
	ACT_CSGO_FIRE_PRIMARY,
	ACT_CSGO_FIRE_PRIMARY_OPT_1,
	ACT_CSGO_FIRE_PRIMARY_OPT_2,
	ACT_CSGO_FIRE_SECONDARY,
	ACT_CSGO_FIRE_SECONDARY_OPT_1,
	ACT_CSGO_FIRE_SECONDARY_OPT_2,
	ACT_CSGO_RELOAD,
	ACT_CSGO_RELOAD_START,
	ACT_CSGO_RELOAD_LOOP,
	ACT_CSGO_RELOAD_END,
	ACT_CSGO_OPERATE,
	ACT_CSGO_DEPLOY,
	ACT_CSGO_CATCH,
	ACT_CSGO_SILENCER_DETACH,
	ACT_CSGO_SILENCER_ATTACH,
	ACT_CSGO_TWITCH,
	ACT_CSGO_TWITCH_BUY_ZONE,
	ACT_CSGO_PLANT_BOMB,
	ACT_CSGO_IDLE_TURN_BALANCE_ADJUST,
	ACT_CSGO_IDLE_ADJUST_STOPPED_MOVING,
	ACT_CSGO_ALIVE_LOOP,
	ACT_CSGO_FLINCH,
	ACT_CSGO_FLINCH_HEAD,
	ACT_CSGO_FLINCH_MOLOTOV,
	ACT_CSGO_JUMP,
	ACT_CSGO_FALL,
	ACT_CSGO_CLIMB_LADDER,
	ACT_CSGO_LAND_LIGHT,
	ACT_CSGO_LAND_HEAVY,
	ACT_CSGO_EXIT_LADDER_TOP,
	ACT_CSGO_EXIT_LADDER_BOTTOM
};

#pragma endregion

class base_animating;
class base_animating_overlay;
class weapon_cs_base;
class cs_player;

class game_bone_accessor
{
public:
	const base_animating*	animating;
	matrix_aligned*			matrix_bones;	// readable/writable bones
	int						readable_bones; // mask of which bones can be read
	int						writable_bones; // mask of which bones can be written
};

class game_animation_layer
{
public:
	float					animation_time; // 0x00
	float					fade_out_time; // 0x04
	game_studio_hdr*		dispatched_studio_hdr; // 0x08
	int						dispatched_source; // 0x0c
	int						dispatched_destination; // 0x10
	int						order; // 0x14
	int						sequence; // 0x18 // in range: [-1 .. 65535]
	float					previous_cycle; // 0x1c // in range: [-2.0 .. 2.0]
	float					weight; // 0x20 // in range: [-5.0 .. 5.0]
	float					weight_delta_rate; // 0x24 // in range: [-5.0 .. 5.0]
	float					playback_rate; // 0x28
	float					cycle; // 0x2c // in range: [-2.0 .. 2.0]
	base_animating_overlay* owner; // 0x30
	int						invalidate_physics_bits; // 0x34
};

class game_animation_state
{
public:
	game_animation_state(cs_player* player);

	void update(const q_angle& view_angle);
	void reset();
	void modify_eye_position(vector_3d& input_eye_position) const;

public:
	const int*				layer_order_preset; // 0x0000
	bool					first_run; // 0x0004
	bool					first_foot_plant; // 0x0005
	int						last_procedural_foot_plant_update_frame; // 0x0008
	float					eye_position_smooth_lerp; // 0x000c
	float					strafe_change_weight_smooth_falloff; // 0x0010
	std::byte				_pad0[0x3c]; // 0x0014
	int						cached_model_index; // 0x0050
	float					step_height_left; // 0x0054
	float					step_height_right; // 0x0058
	weapon_cs_base*			last_weapon_bone_setup; // 0x005c
	cs_player*				player; // 0x0060
	weapon_cs_base*			weapon; // 0x0064
	weapon_cs_base*			last_weapon; // 0x0068
	float					last_update_time; // 0x006c
	int						last_update_frame; // 0x0070
	float					last_update_increment; // 0x0074
	float					eye_yaw; // 0x0078
	float					eye_pitch; // 0x007c
	float					foot_yaw; // 0x0080
	float					last_foot_yaw; // 0x0084
	float					move_yaw; // 0x0088
	float					move_yaw_ideal; // 0x008c // changes when moving/jumping/hitting ground
	float					move_yaw_current_to_ideal; // 0x0090
	float					time_to_align_lower_body; // 0x0094
	float					primary_cycle; // 0x0098 // in range: [0.0 .. 1.0]
	float					move_weight; // 0x009c // in range: [0.0 .. 1.0]
	float					move_weight_smoothed; // 0x00a0
	float					duck_amount; // 0x00a4
	float					duck_additional; // 0x00a8 // for when we duck a bit after hitting ground from a jump
	float					recrouch_weight; // 0x00ac
	vector_3d				origin; // 0x00b0
	vector_3d				last_origin;// 0x00bc
	vector_3d				velocity; // 0x00c8
	vector_3d				velocity_normalized; // 0x00d4
	vector_3d				velocity_normalized_non_zero; // 0x00e0
	float					velocity_length_2d; // 0x00ec
	float					velocity_length_z; // 0x00f0
	float					run_speed_normalized; // 0x00f4 // @ida: client.dll -> ["f3 0f 59 8f ? ? ? ? 0f 2f" + 0x4]
	float					walk_speed_normalized; // 0x00f8
	float					crouch_speed_normalized; // 0x00fc
	float					moving_duration; // 0x0100
	float					duration_still; // 0x0104
	bool					on_ground; // 0x0108 // @ida: client.dll -> abs["e8 ? ? ? ? f6 86 ? ? ? ? ? 0f 84" + 0x1] + 0x18
	bool					landing; // 0x0109
	float					jump_to_fall; // 0x010c
	float					duration_in_air; // 0x0110
	float					left_ground_height; // 0x0114
	float					hit_ground_weight; // 0x0118
	float					walk_to_run_transition; // 0x011c // in range: [0.0 .. 1.0], doesn't change when walking or crouching, only running // @ida: client.dll -> ["f3 0f 10 9e ? ? ? ? 0f 57 c9" + 0x4]
	std::byte				_pad1[0x4]; // 0x0120
	float					in_air_smooth_value; // 0x0124 // in range: [0.0 .. 1.0], affected while jumping and running, or when just jumping
	bool					on_ladder; // 0x0128
	float					ladder_weight; // 0x012c
	float					ladder_speed; // 0x0130
	bool					walk_to_run_transition_state; // 0x0134
	bool					defuse_started; // 0x0135
	bool					plant_animation_started; // 0x0136
	bool					twitch_animation_started; // 0x0137
	bool					adjustment_started; // 0x0138
	utl_vector<utl_symbol>	activity_modifiers; // 0x013c
	float					next_twitch_time; // 0x0150
	float					time_of_last_known_injury; // 0x0154
	float					last_velocity_test_time; // 0x0158 // @ida: client.dll -> ["f3 0f 5c a7 ? ? ? ? 0f 2f" + 0x4]
	vector_3d				last_velocity; // 0x015c
	vector_3d				target_acceleration; // 0x0168
	vector_3d				acceleration; // 0x0174 // @ida: client.dll -> ["f3 0f 7e 87 ? ? ? ? 8d b7" + 0x4]
	float					acceleration_weight; // 0x0180
	std::byte				_pad2[0xc]; // 0x184
	float					strafe_change_weight; // 0x0190 // @ida: client.dll -> ["f3 0f 10 9e ? ? ? ? 0f 28 ec" + 0x4]
	float					strafe_change_target_weight; // 0x194
	float					strafe_change_cycle; // 0x0198 // @ida: client.dll -> ["f3 0f 10 96 ? ? ? ? f3 0f 59 25" + 0x4]
	int						strafe_sequence; // 0x019c
	bool					strafe_changing; // 0x01a0
	float					strafing_duration; // 0x01a4
	float					foot_lerp; // 0x01a8
	bool					feet_crossed; // 0x01ac
	bool					player_is_accelerating; // 0x01ad // @ida: client.dll -> ["88 86 ? ? ? ? 8b 86 ? ? ? ? 83" + 0x2]
	std::byte				_pad3[0x174 + 0x2]; // 0x01ae
	float					camera_smooth_height; // 0x0324
	bool					smooth_height_valid; // 0x0328 @ida: client.dll -> ["c6 87 ? ? ? ? ? eb 92" + 0x2]
	std::byte				_pad4[0x10 + 0x3]; // 0x0329
	float					aim_yaw_min; // 0x033c // @ida: client.dll -> ["f3 0f 5e 86 ? ? ? ? f3 0f 59 05 ? ? ? ? 80" + 0x4]
	float					aim_yaw_max; // 0x0340
	int						animation_set_version; // 0x0344 // @xref: "animset_version"
};
