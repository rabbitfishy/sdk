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

int base_animating::bone_by_hash(const hash32 bone_hash)
{
	game_studio_hdr* model_hdr = this->model_ptr();

	if (model_hdr != nullptr)
	{
		for (int i = 0; i < model_hdr->bone_count; i++)
		{
			game_studio_bone* bone = model_hdr->bone(i);

			if (bone != nullptr && HASH_COMPILE(bone->name()) == bone_hash)
				return i;
		}
	}

	// given invalid bone hash or model header is corrupted.
	assert(false);
	return game_bone_index::bone_invalid;
}

vector_3d base_animating::bone_position(int bone_index)
{
	game_studio_hdr* model_hdr = this->model_ptr();

	if (model_hdr != nullptr && bone_index >= 0 && bone_index < model_hdr->bone_count)
	{
		if (!this->setup_bones(nullptr, -1, bone_used_by_anything, interfaces->global_vars->current_time))
			return vector_3d();

		const game_bone_accessor& accessor = this->bone_accessor();
		return accessor.matrix_bones[bone_index].get_origin();
	}

	// given invalid bone index or model header is corrupted.
	assert(false);
	return vector_3d();
}

vector_3d cs_player::shoot_position()
{
	vector_3d position = this->eye_position();

	if (this->is_new_animation_state())
	{
		const game_animation_state* animation_state = this->animation_state();
		
		if (!animation_state)
			animation_state->modify_eye_position(position);
	}

	return position;
}
