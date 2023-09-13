#pragma once

class game_weapon_system
{
public:
	game_weapon_info*	weapon_data(short item_definition_index) { return virtuals->call<game_weapon_info*>(this, 2, item_definition_index); };
};