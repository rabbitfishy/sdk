#pragma once

#pragma region game types flags

enum game_type_flags : int
{
	game_type_unknown = -1,
	game_type_classic,
	game_type_gungame,
	game_type_training,
	game_type_custom,
	game_type_cooperative,
	game_type_skirmish,
	game_type_free_for_all
};

// credit: https://developer.valvesoftware.com/wiki/CSGO_Game_Mode_Commands
enum game_mode_flags : int
{
	game_mode_unknown = 0,
	game_mode_casual,
	game_mode_competitive,
	game_mode_wingman,
	game_mode_arms_race,
	game_mode_demolition,
	game_mode_deathmatch,
	game_mode_guardian,
	game_mode_coopstrike,
	game_mode_danger_zone
};

#pragma endregion

class game_client_types
{
public:
	int				current_game_type() { return virtuals->call<int>(this, 8); }
	int				current_game_mode() { return virtuals->call<int>(this, 9); }
	const char*		current_map_name() { return virtuals->call<const char*>(this, 10); }
	const char*		current_game_type_name_id() { return virtuals->call<const char*>(this, 11); }
	const char*		current_game_mode_name_id() { return virtuals->call<const char*>(this, 13); }
};