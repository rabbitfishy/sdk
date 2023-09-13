#pragma once
#include "../../../dependencies/other/hash.h"

#pragma region engine client flags

enum client_frame_stage : int
{
	frame_undefined = -1,
	frame_start,

	// a network packet is being recieved.
	frame_net_update_start,

	// data has been received and we are going to start calling postdataupdate.
	frame_net_update_post_data_update_start,

	// data has been received and called postdataupdate on all data recipients.
	frame_net_update_post_data_update_end,

	// received all packets, we can now do interpolation, prediction, etc...
	frame_net_update_end,

	// start rendering the scene.
	frame_render_start,

	// finished rendering the scene.
	frame_render_end,
	frame_net_full_frame_update_on_remove
};

#pragma endregion

struct game_player_info
{
	std::uint64_t	version;
	std::uint64_t	xuid;
	char			name[128];
	int				user_id;
	char			steam_id[33];
	std::uint32_t	friends_id;
	char			friends_name[128];
	bool			fake_player;
	bool			is_hltv;
	checksum32		custom_files[4];
	std::uint8_t	files_downloaded;
};

class game_engine_client
{
public:
	void					get_screen_size(int& width, int& height) { return virtuals->call<void>(this, 5, std::ref(width), std::ref(height)); }
	bool					get_player_info(int index, game_player_info* info) { return virtuals->call<bool>(this, 8, index, info); }
	int						get_player_uid(int uid) { return virtuals->call<int>(this, 9, uid); }
	bool					is_console_visible() { return virtuals->call<bool>(this, 11); }
	int						get_local_player() { return virtuals->call<int>(this, 12); }
	void					get_view_angles(q_angle& view_angle) { return virtuals->call<void>(this, 18, std::ref(view_angle)); }
	void					set_view_angles(q_angle& view_angle) { return virtuals->call<void>(this, 19, std::ref(view_angle)); }
	bool					in_game() { return virtuals->call<bool>(this, 26); }
	view_matrix&			world_to_screen_matrix() { return virtuals->call<view_matrix&>(this, 37); }
	game_net_channel_info*	net_info() { return virtuals->call<game_net_channel_info*>(this, 78); }
};