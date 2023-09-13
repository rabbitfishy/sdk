#include "../hooks.h"

void test_esp()
{
	if (!interfaces->engine_client->in_game())
		return;

	if (!csgo->local_player)
		return;

	for (int i = 1; i < interfaces->global_vars->max_clients; i++)
	{
		cs_player* entity = interfaces->entity_list->get<cs_player>(i);

		if (!csgo->local_player)
			continue;

		if (!entity || !entity->alive() || entity->team() == csgo->local_player->team() || entity->dormant())
			continue;

		matrix_3x4 bones[128];
		if (!entity->setup_bones(bones, 128, 0x7FF00, interfaces->global_vars->current_time))
			continue;

		// player head position.
		vector_3d top = { };
		if (interfaces->debug_overlay->screen_position(bones[8].get_origin() + vector_3d{ 0.f, 0.f, 11.f }, top))
			return;

		// player feet position.
		vector_3d bottom = { };
		if (interfaces->debug_overlay->screen_position(entity->abs_origin() - vector_3d{ 0.f, 0.f, 9.f }, bottom))
			return;

		const int h = bottom.y - top.y;
		const int w = h * 0.3f;
		const int left = (top.x - w);
		const int right = (top.x + w);

		rect box = { left, (int)top.y, right - left, h };

		render->outlined_rect(box.x, box.y, box.w, box.h, color(255, 255, 255));
	}
}

void __stdcall hk_functions::paint_traverse(v_panel panel, bool force_repaint, bool allow_force)
{
	hash32 draw_panel = HASH_COMPILE(interfaces->panel->name(panel));

	switch (draw_panel)
	{
	case HASH_COMPILE("MatSystemTopPanel"):
		// drawing functions here.

		test_esp();

		break;
	}

	originals->paint_traverse_original(interfaces->panel, panel, force_repaint, allow_force);
}