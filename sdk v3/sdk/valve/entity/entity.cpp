#include "entity.h"

base_combat_weapon* base_player::active_weapon()
{
	if (!this)
		return nullptr;

	return interfaces->entity_list->get_handle<base_combat_weapon>(this->active_weapon_handle());
}

game_weapon_info* base_combat_weapon::weapon_data()
{
	if (!this)
		return nullptr;

	return interfaces->weapon_system->weapon_data(this->item_definition_index());
}