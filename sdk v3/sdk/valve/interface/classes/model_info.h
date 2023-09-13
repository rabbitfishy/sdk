#pragma once

#pragma region model info flags

enum game_render_flags : unsigned int
{
	render_flags_disable_rendering			= 0x01,
	render_flags_has_changed				= 0x02,
	render_flags_alternate_sorting			= 0x04,
	render_flags_render_with_viewmodels		= 0x08,
	render_flags_bloat_bounds				= 0x10,
	render_flags_bounds_valid				= 0x20,
	render_flags_bounds_always_recompute	= 0x40,
	render_flags_is_sprite					= 0x80,
	render_flags_force_opaque_pass			= 0x100,
};

#pragma endregion

struct game_model
{
	std::byte		_pad0[0x4];
	char			name[260];
	int				load_flags;
	int				server_count;
	int				type;
	int				flags;
	vector_3d		mins;
	vector_3d		maxs;
	float			radius;
	std::byte		_pad1[0x1C];
};

class game_model_info
{
public:
	const char*			model_name(const game_model* model) noexcept { return virtuals->call<const char*>(this, 3, model); }
	game_studio_hdr*	studio_model(const game_model* model) noexcept { return virtuals->call<game_studio_hdr*>(this, 32, model); }
};