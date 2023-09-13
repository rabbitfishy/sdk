#pragma once
#include "../maths/q_angle.h"
#include "../../dependencies/memory/modules.h"
#include "../../dependencies/memory/scanner.h"
#include "../../dependencies/memory/signatures.h"
#include "../../dependencies/memory/virtual.h"
#include "../../dependencies/other/hash.h"
#include "../maths/vector.h"

#pragma region user cmd flags

enum command_buttons : int
{
	in_attack			= (1 << 0),
	in_jump				= (1 << 1),
	in_duck				= (1 << 2),
	in_forward			= (1 << 3),
	in_back				= (1 << 4),
	in_use				= (1 << 5),
	in_cancel			= (1 << 6),
	in_left				= (1 << 7),
	in_right			= (1 << 8),
	in_move_left		= (1 << 9),
	in_move_right		= (1 << 10),
	in_second_attack	= (1 << 11),
	in_run				= (1 << 12),
	in_reload			= (1 << 13),
	in_left_alt			= (1 << 14),
	in_right_alt		= (1 << 15),
	in_score			= (1 << 16),
	in_speed			= (1 << 17),
	in_walk				= (1 << 18),
	in_zoom				= (1 << 19),
	in_first_weapon		= (1 << 20),
	in_second_weapon	= (1 << 21),
	in_bullrush			= (1 << 22),
	in_first_grenade	= (1 << 23),
	in_second_grenade	= (1 << 24),
	in_middle_attack	= (1 << 25),
	in_use_or_reload	= (1 << 26)
};

#pragma endregion

class game_user_cmd
{
public:
	virtual			~game_user_cmd() { }	// 0x00
	int				command_number;			// 0x04
	int				tick_count;				// 0x08
	q_angle			view_angles;			// 0x0C
	vector_3d		aim_direction;			// 0x18
	float			forward_move;			// 0x24
	float			side_move;				// 0x28
	float			up_move;				// 0x2C
	int				buttons;				// 0x30
	std::uint8_t	impulse;				// 0x34
	int				weapon_select;			// 0x38
	int				weapon_sub_type;		// 0x3C
	int				random_seed;			// 0x40
	short			mouse_x;				// 0x44
	short			mouse_y;				// 0x46
	bool			has_been_predicted;		// 0x48
	q_angle			head_angles;			// 0x4C
	vector_3d		head_offset;			// 0x58

	/*
	* NOTE: this no longer needed anymore.
	* thanks to lagcomp csgo_sdk for the checksum signature pattern and how it's being used.
	* credit: https://github.com/lagcomp/csgo_sdk/blob/84b1557347e1c14b73e808048c8366ebe6053fd7/csgo_sdk/csgo/ctx/impl/ctx.cpp#L275
	* credit: https://github.com/lagcomp/csgo_sdk/blob/84b1557347e1c14b73e808048c8366ebe6053fd7/csgo_sdk/csgo/valve/other/other.hpp#L135
	*/
	checksum32 checksum() const 
	{
		checksum32 check = 0UL;
		check = CHECKSUM(command_number, sizeof(command_number));
		check = CHECKSUM(tick_count, sizeof(tick_count));
		check = CHECKSUM(view_angles, sizeof(view_angles));
		check = CHECKSUM(aim_direction, sizeof(aim_direction));
		check = CHECKSUM(forward_move, sizeof(forward_move));
		check = CHECKSUM(side_move, sizeof(side_move));
		check = CHECKSUM(up_move, sizeof(up_move));
		check = CHECKSUM(buttons, sizeof(buttons));
		check = CHECKSUM(impulse, sizeof(impulse));
		check = CHECKSUM(weapon_select, sizeof(weapon_select));
		check = CHECKSUM(weapon_sub_type, sizeof(weapon_sub_type));
		check = CHECKSUM(random_seed, sizeof(random_seed));
		check = CHECKSUM(mouse_x, sizeof(mouse_x));
		check = CHECKSUM(mouse_y, sizeof(mouse_y));
		check = CHECKSUM(has_been_predicted, sizeof(has_been_predicted));
		check = CHECKSUM(head_angles, sizeof(head_angles));
		check = CHECKSUM(head_offset, sizeof(head_offset));
		return check;
	}
};

class game_verified_user_cmd
{
public:
	game_user_cmd	cmd;	    // 0x00
	checksum32		checksum;	// 0x64
};