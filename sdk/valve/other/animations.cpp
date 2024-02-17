#include "animations.h"
#include "../../dependencies/dependency.h"

game_animation_state::game_animation_state(cs_player* player)
{
	using constructor_details = void(__thiscall*)(game_animation_state*, cs_player*);
	static auto original_constructor = SEARCH(modules->client, signatures::other::animation_state_constructor::signature()).cast<constructor_details>();
	original_constructor(this, player);
}

void game_animation_state::update(const q_angle& view_angle)
{
	using update_details = void(__vectorcall*)(game_animation_state*, void*, float, float, float, void*);
	static auto original_constructor = SEARCH(modules->client, signatures::other::animation_state_update::signature()).cast<update_details>();
	original_constructor(this, nullptr, view_angle.z, view_angle.y, view_angle.x, nullptr);
}

void game_animation_state::reset()
{
	using reset_details = void(__thiscall*)(game_animation_state*);
	static auto original_constructor = SEARCH(modules->client, signatures::other::animation_state_reset::signature()).cast<reset_details>();
	original_constructor(this);
}

void game_animation_state::modify_eye_position(vector_3d& input_eye_position) const
{
	if (!this->player || interfaces->engine_client->hltv())
		return;

	if (!this->landing && this->duck_amount == 0.f && this->player->ground_entity_handle().valid())
		return;

	const int head_bone = this->player->bone_by_hash(HASH_COMPILE("head_0"));
	if (head_bone == game_bone_index::bone_invalid)
		return;

	vector_3d head_position = this->player->bone_position(head_bone);
	head_position.z += 1.7f;

	if (input_eye_position.z > head_position.z)
	{
		const float height_factor = std::clamp((std::fabsf(input_eye_position.z - head_position.z) - 4.f) / 6.f, 0.f, 1.f);
		// simple spline remapval clamped.
		const float height_factor_sqr = (height_factor * height_factor);
		input_eye_position.z += ((head_position.z - input_eye_position.z) * ((height_factor_sqr * 3.f) - ((height_factor_sqr * 2.f) * height_factor)));
	}
}