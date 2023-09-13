#pragma once

class game_move_data
{
public:
	bool			first_run_instructions : 1;
	bool			game_code_moved_player : 1;
	bool			no_air_control : 1;
	std::uintptr_t	player_handle;			// edict index on server, client entity handle on client=
	int				impulse_command;		// impulse command issued.
	q_angle			view_angles;			// command view angles (local space)
	q_angle			abs_view_angles;		// command view angles (world space)
	int				buttons;				// attack buttons.
	int				old_buttons;			// from host_client->oldbuttons;
	float			forward_move;
	float			side_move;
	float			up_move;
	float			max_speed;
	float			client_max_speed;
	vector_3d		velocity;				// edict::velocity	// current movement direction.
	vector_3d		trailing_velocity;
	float			trailing_velocity_time;
	vector_3d		angles;					// edict::angles
	vector_3d		old_angles;
	float			out_step_height;		// how much you climbed this move
	vector_3d		out_wish_velocity;		// this is where you tried 
	vector_3d		out_jump_velocity;		// this is your jump velocity
	vector_3d		constraint_center;
	float			constraint_radius;
	float			constraint_width;
	float			constraint_speed_factor;
	bool			constraint_past_radius;
	vector_3d		abs_origin;
};

class game_move_helper
{
public:
	virtual	const char*						name(void* entity) const = 0;
	virtual void							set_host(base_entity* phost) = 0;
	virtual void							reset_touch_list() = 0;
	virtual bool							add_to_touched(const game_trace& trace, const vector_3d& impact_velocity) = 0;
	virtual void							process_impacts() = 0;
	virtual void							con_printf(int nindex, char const* fmt, ...) = 0;
	virtual void							start_sound(const vector_3d& origin, int channel, char const* sample, float volume, game_sound_level sound_level, int flags, int pitch) = 0;
	virtual void							start_sound(const vector_3d& origin, const char* sound_name) = 0;
	virtual void							playback_event_full(int flags, int client_index, unsigned short event_index, float delay, vector_3d& origin, vector_3d& angles, float fl_param1, float fl_param2, int i_param1, int i_param2, int b_param1, int b_param2) = 0;
	virtual bool							player_falling_damage() = 0;
	virtual void							player_set_animation(int player_animation) = 0;
	virtual game_physics_surface_props*		surface_props() = 0;
	virtual bool							is_world_entity(const unsigned long& entity) = 0;
};

class game_movement
{
public:
	virtual						~game_movement() { }
	virtual void				process_movement(base_entity* entity, game_move_data* move) = 0;
	virtual void				reset() = 0;
	virtual void				start_track_prediction_errors(base_entity* entity) = 0;
	virtual void				finish_track_prediction_errors(base_entity* entity) = 0;
	virtual void				diff_print(char const* fmt, ...) = 0;
	virtual vector_3d const&	player_mins(bool ducked) const = 0;
	virtual vector_3d const&	player_maxs(bool ducked) const = 0;
	virtual vector_3d const&	player_view_offset(bool ducked) const = 0;
	virtual bool				is_moving_player_stuck() const = 0;
	virtual base_entity*		moving_player() const = 0;
	virtual void				unblock_pusher(base_entity* entity, base_entity* pusher) = 0;
	virtual void				setup_movement_bounds(game_move_data* move) = 0;
};

class game_prediction
{
public:
	std::byte		_pad0[0x4];						// 0x0000
	std::uintptr_t	last_ground;					// 0x0004
	bool			in_prediction;					// 0x0008
	bool			is_first_time_predicted;		// 0x0009
	bool			engine_paused;					// 0x000a
	bool			old_cl_predict_value;			// 0x000b
	int				previous_start_frame;			// 0x000c
	int				incoming_packet_number;			// 0x0010
	float			last_server_world_time_stamp;	// 0x0014

	struct game_split
	{
		bool						is_first_time_predicted;						// 0x0018
		std::byte					_pad0[0x3];										// 0x0019
		int							commands_predicted;								// 0x001c
		int							server_commands_acknowledged;					// 0x0020
		int							previous_ack_had_errors;						// 0x0024
		float						ideal_pitch;									// 0x0028
		int							last_command_acknowledged;						// 0x002c
		bool						previous_ack_error_triggers_full_latch_reset;	// 0x0030
		utl_vector<base_handle>		entities_with_prediction_errors_in_last_ack;	// 0x0031
		bool						performed_tick_shift;							// 0x0045
	};

	game_split		split[1];	// 0x0018

public:

	void update(int start_frame, bool valid_frame, int incoming_acknowledged, int outgoing_command) { return virtuals->call<void>(this, 3, start_frame, valid_frame, incoming_acknowledged, outgoing_command); }
	void local_view_angles(q_angle& view_angle) { return virtuals->call<void>(this, 12, std::ref(view_angle)); }
	void set_local_view_angles(q_angle& view_angle) { return virtuals->call<void>(this, 13, std::ref(view_angle)); }
	void check_moving_ground(base_entity* entity, double frame_time) { return virtuals->call<void>(this, 18, entity, frame_time); }
	void setup_move(base_entity* entity, game_user_cmd* cmd, game_move_helper* helper, game_move_data* move_data) { return virtuals->call<void>(this, 20, entity, cmd, helper, move_data); }
	void finish_move(base_entity* entity, game_user_cmd* cmd, game_move_data* move_data) { return virtuals->call<void>(this, 21, entity, cmd, move_data); }
};