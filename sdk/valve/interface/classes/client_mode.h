#pragma once

#pragma region signon state definitions

#define signon_state_none			0	// no state yet, about to connect
#define signon_state_challenge		1	// client challenging server, all oob packets
#define signon_state_connected		2	// client is connected to server, netchans ready
#define signon_state_new			3	// just got serverinfo and string tables
#define signon_state_pre_spawn		4	// received signon buffers
#define signon_state_spawn			5	// ready to receive entity packets
#define signon_state_full			6	// we are fully connected, first non-delta packet received (in-game check)
#define signon_state_change_level	7	// server is changing level, please wait

#pragma endregion

typedef DWORD h_cursor;

class game_view_setup
{
public:
	int			x;
	int			unscaled_x;
	int			y;
	int			unscaled_y;
	int			width;
	int			unscaled_width;
	int			height;
	int			unscaled_height;
	bool		ortho;
	float		ortho_left;
	float		ortho_top;
	float		ortho_right;
	float		ortho_bottom;
	std::byte	_pad0[0x7c];
	float		fov;
	float		viewmodel_fov;
	vector_3d	origin;
	q_angle		view_angle;
	float		near_z;
	float		far_z;
	float		near_viewmodel_z;
	float		far_viewmodel_z;
	float		aspect_ratio;
	float		near_blur_depth;
	float		near_focus_depth;
	float		far_focus_depth;
	float		far_blur_depth;
	float		near_blur_radius;
	float		far_blur_radius;
	float		do_fquality;
	int			motion_blur_mode;
	float		shutter_time;
	vector_3d	shutter_open_position;
	q_angle		shutter_open_angles;
	vector_3d	shutter_close_position;
	q_angle		shutter_close_angles;
	float		off_center_top;
	float		off_center_bottom;
	float		off_center_left;
	float		off_center_right;
	bool		off_center : 1;
	bool		render_to_subrect_of_larger_screen : 1;
	bool		do_bloom_and_tone_mapping : 1;
	bool		do_depth_of_field : 1;
	bool		hdr_target : 1;
	bool		draw_world_normal : 1;
	bool		cull_font_faces : 1;
	bool		cache_fullscene_state : 1;
	bool		csm_view : 1;
};

class game_hud_chat;
class game_client_mode
{
public:
	std::byte		_pad0[0x1b];
	void*			view_port;
	game_hud_chat*	chat_element;
	h_cursor		cursor_none;
	void*			weapon_selection;
	int				root_size[2];
};

class game_app_system
{
private:
	virtual void function0() = 0;
	virtual void function1() = 0;
	virtual void function2() = 0;
	virtual void function3() = 0;
	virtual void function4() = 0;
	virtual void function5() = 0;
	virtual void function6() = 0;
	virtual void function7() = 0;
	virtual void function8() = 0;
};

class game_event_info
{
public:
	short						class_id;		// 0x00 // 0 implies not in use
	float						fire_delay;		// 0x02 // if non-zero, the delay time when the event should be fired ( fixed up on the client )
	const void*					send_table;		// 0x06
	const game_client_class*	client_class;	// 0x0a // clienclass pointer
	void*						data;			// 0x0e // raw event data
	std::intptr_t				packed_bits;	// 0x12
	int							flags;			// 0x16
	std::byte					_pad0[0x16];	// 0x1a
};

class game_client_state
{
public:
	std::byte			_pad0[0x9c];				// 0x0000
	game_net_channel*	net_channel;				// 0x009c
	int					challenge;					// 0x00a0
	std::byte			_pad1[0x64];				// 0x00a4
	int					signon_state;				// 0x0108
	std::byte			_pad2[0x8];					// 0x010c
	float				next_cmd_time;				// 0x0114
	int					server_count;				// 0x0118
	int					current_sequence;			// 0x011c
	std::byte			_pad3[0x54];				// 0x0120
	int					delta_tick;					// 0x0174
	bool				paused;						// 0x0178
	std::byte			_pad4[0x7];					// 0x0179
	int					view_entity;				// 0x0180
	int					player_slot;				// 0x0184
	char				level_name[MAX_PATH];		// 0x0188
	char				level_name_short[80];		// 0x028c
	char				map_group_name[80];			// 0x02dc
	char				last_level_name_short[80];	// 0x032c
	std::byte			_pad5[0xc];					// 0x037c
	int					max_clients;				// 0x0388 
	std::byte			_pad6[0x498c];				// 0x038c
	float				last_server_tick_time;		// 0x4d18
	bool				in_simulation;				// 0x4d1c
	std::byte			_pad7[0x3];					// 0x4d1d
	int					old_tick_count;				// 0x4d20
	float				tick_remainder;				// 0x4d24
	float				frame_time;					// 0x4d28
	int					last_out_going_command;		// 0x4d2c
	int					choked_commands;			// 0x4d30
	int					last_command_ack;			// 0x4d34
	int					command_ack;				// 0x4d38
	int					sound_sequence;				// 0x4d3c
	std::byte			_pad8[0x50];				// 0x4d40
	q_angle				view_angle;					// 0x4d90
	std::byte			_pad9[0xd0];				// 0x4d9c
	game_event_info*	events;						// 0x4e6c
};