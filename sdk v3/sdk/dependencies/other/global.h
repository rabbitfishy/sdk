#pragma once

class cs_player;

struct game_global
{
	cs_player* local_player		= nullptr;
	game_user_cmd* cmd			= nullptr;
	bool* send_packet			= nullptr;
};

extern game_global* csgo;