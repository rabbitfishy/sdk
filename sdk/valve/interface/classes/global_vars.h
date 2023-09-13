#pragma once

class game_global_vars
{
public:
	float	real_time;
	int		frame_count;
	float	absolute_frame_time;
	float	absolute_frame_start_time;
	float	current_time;
	float	frame_time;
	int		max_clients;
	int		tick_count;
	float	interval_per_tick;
	float	interpolation_amount;
	int		frame_simulation_ticks;
	int		network_protocol;
	void*	save_data;
	bool	client;
	bool	remote_client;
	int		timestamp_networking_base;
	int		timestamp_randomize_window;
};