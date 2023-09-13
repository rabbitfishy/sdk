#pragma once

#pragma region studio render flags

enum
{
	studio_render_draw_entire_model										= 0,
	studio_render_draw_opaque_only										= 0x01,
	studio_render_draw_translucent_only									= 0x02,
	studio_render_draw_group_mask										= 0x03,
	studio_render_draw_no_flexes										= 0x04,
	studio_render_draw_static_lighting									= 0x08,
	studio_render_draw_accurate_time									= 0x10,		// use accurate timing when drawing the model.
	studio_render_draw_no_shadows										= 0x20,
	studio_render_draw_get_perf_stats									= 0x40,
	studio_render_draw_wireframe										= 0x80,
	studio_render_draw_item_blink										= 0x100,
	studio_render_shadow_depth_texture									= 0x200,
	studio_render_unused												= 0x400,
	studio_render_skip_decals											= 0x800,
	studio_render_model_is_cacheable									= 0x1000,
	studio_render_shadow_depth_texture_include_translucent_materials	= 0x2000,
	studio_render_no_primary_draw										= 0x4000,
	studio_render_ssao_depth_texture									= 0x8000,
};

enum game_override_type : int
{
	override_normal = 0,
	override_build_shadows,
	override_depth_write,
	override_selective,
	override_ssao_depth_write,
};

#pragma endregion

class game_studio_hdr;
class client_renderable;
class game_draw_model_info
{
public:
	game_studio_hdr*	studio_hdr;
	void*				hardware_data;
	int					decals;
	int					skin;
	int					body;
	int					hitbox_set;
	client_renderable*	renderable;
};

class game_studio_render
{
public:
	void		set_color_modulation(const float* color) noexcept { return virtuals->call<void>(this, 27, color); }
	void		set_alpha_modulation(const float alpha) noexcept { return virtuals->call<void>(this, 28, alpha); }
	void		forced_material_override(game_material* material, const int type = override_normal, const int index = -1) noexcept { return virtuals->call<void>(this, 33, material, type, index); }
	bool		is_forced_material_override() noexcept { return virtuals->call<bool>(this, 34); }
};