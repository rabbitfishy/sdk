#pragma once

#pragma region material flags

enum game_material_flags : int
{
	material_debug							= (1 << 0),
	material_no_debug_override				= (1 << 1),
	material_no_draw						= (1 << 2),
	material_use_in_fillrate_mode			= (1 << 3),
	material_vertex_color					= (1 << 4),
	material_vertex_alpha					= (1 << 5),
	material_selfillum						= (1 << 6),
	material_additive						= (1 << 7),
	material_alpha_test						= (1 << 8),
	material_pseudo_translucent				= (1 << 9),		// used to mark water materials for rendering after opaques but before translucents (with alpha blending but also with depth writes)
	material_znearer						= (1 << 10),
	material_model							= (1 << 11),
	material_flat							= (1 << 12),
	material_nocull							= (1 << 13),
	material_nofog							= (1 << 14),
	material_ignorez						= (1 << 15),
	material_decal							= (1 << 16),
	material_envmap_sphere					= (1 << 17),	// obsolete
	material_aoprepass						= (1 << 18),
	material_envmap_camera_space			= (1 << 19),	// obsolete
	material_base_alpha_envmapmask			= (1 << 20),
	material_translucent					= (1 << 21),
	material_normal_map_alpha_envmapmask	= (1 << 22),
	material_needs_software_skinning		= (1 << 23),	// obsolete
	material_opaque_texture					= (1 << 24),
	material_multiply						= (1 << 25),
	material_suppress_decals				= (1 << 26),
	material_half_lambert					= (1 << 27),
	material_wireframe						= (1 << 28),
	material_allow_alpha_to_coverage		= (1 << 29),
	material_alpha_modified_by_proxy		= (1 << 30),
	material_vertex_fog						= (1 << 31)
};

#pragma endregion

class game_material
{
public:
	void				set_material_flags(const int flags, const bool enable) noexcept { return virtuals->call<void>(this, 29, flags, enable); }
};

class game_material_system
{
public:
	game_material*		create_material(const char* name, game_key_value* key) noexcept { return virtuals->call<game_material*>(this, 83, name, key); }
	game_material*		find(const char* name) noexcept { return virtuals->call<game_material*>(this, 84, name, nullptr, true, nullptr); }
};