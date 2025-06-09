#pragma once
#include "../../maths/utl_vector.h"
#include "../../../dependencies/memory/modules.h"
#include "../../../dependencies/memory/signatures.h"
#include "../../../dependencies/memory/virtual.h"

#pragma region engine trace flags

enum game_char_texture
{
	char_texture_antlion		= 'A',
	char_texture_bloody_flesh	= 'B',
	char_texture_concrete		= 'C',
	char_texture_dirt			= 'D',
	char_texture_egg_shell		= 'E',
	char_texture_flesh			= 'F',
	char_texture_grate			= 'G',
	char_texture_alien_flesh	= 'H',
	char_texture_clip			= 'I',
	char_texture_snow			= 'K',
	char_texture_plastic		= 'L',
	char_texture_metal			= 'M',
	char_texture_sand			= 'N',
	char_texture_foliage		= 'O',
	char_texture_computer		= 'P',
	char_texture_reflective		= 'R',
	char_texture_slosh			= 'S',
	char_texture_tile			= 'T',
	char_texture_cardboard		= 'U',
	char_texture_vent			= 'V',
	char_texture_wood			= 'W',
	char_texture_fake			= 'X',
	char_texture_glass			= 'Y',
	char_texture_warp_shield	= 'Z',
	char_texture_sand_barrel	= '\xC'
};

enum game_damage_type : int
{
	damage_no			= 0,
	damage_events_only	= 1,
	damage_yes			= 2,
	damage_aim			= 3
};

enum game_collision_group : int
{
	collision_none = 0,
	collision_debris,					// collides with nothing but world and static stuff
	collision_debris_trigger,			// same as debris, but hits triggers
	collision_interactive_debris,		// collides with everything except other interactive debris or debris
	collision_interactive,				// collides with everything except interactive debris or debris
	collision_player,
	collision_breakable_glass,
	collision_vehicle,
	collision_player_movement,			// for hl2, same as collision_group_player, for / tf2, this filters out other players and cbaseobjects
	collision_npc,						// generic npc group
	collision_in_vehicle,				// for any entity inside a vehicle
	collision_weapon,					// for any weapons that need collision detection
	collision_vehicle_clip,				// vehicle clip brush to restrict vehicle movement
	collision_projectile,				// projectiles!
	collision_door_blocker,				// blocks entities not permitted to get near moving doors
	collision_passable_door,			// doors that the player shouldn't collide with
	collision_dissolving,				// things that are dissolving are in this group
	collision_push_away,				// nonsolid on client and server, pushaway in player code
	collision_npc_actor,				// used so npcs in scripts ignore the player.
	collision_npc_scripted,				// used for npcs in scripts that should not collide with each other
	collision_pz_clip,
	collision_debris_block_projectile,	// only collides with bullets
	collision_last_shared
};

enum game_trace_type : int
{
	trace_everything = 0,
	trace_world_only,
	trace_entities_only,
	trace_everything_filter_props,
	trace_filter_sky
};

enum game_trace_contents : int
{
	contents_empty					= 0,
	contents_solid					= (1 << 0),
	contents_window					= (1 << 1),
	contents_aux					= (1 << 2),
	contents_grate					= (1 << 3),
	contents_slime					= (1 << 4),
	contents_water					= (1 << 5),
	contents_blocklos				= (1 << 6),
	contents_opaque					= (1 << 7),
	contents_testfogvolum			= (1 << 8),
	contents_unused					= (1 << 9),
	contents_block_light			= (1 << 10),
	contents_team1					= (1 << 11),
	contents_team2					= (1 << 12),
	contents_ignore_nodraw_opaque	= (1 << 13),
	contents_moveable				= (1 << 14),
	contents_area_portal			= (1 << 15),
	contents_player_clip			= (1 << 16),
	contents_monster_clip			= (1 << 17),
	contents_current_0				= (1 << 18),
	contents_current_90				= (1 << 19),
	contents_current_180			= (1 << 20),
	contents_current_270			= (1 << 21),
	contents_current_up				= (1 << 22),
	contents_current_down			= (1 << 23),
	contents_origin					= (1 << 24),
	contents_monster				= (1 << 25),
	contents_debris					= (1 << 26),
	contents_detail					= (1 << 27),
	contents_translucent			= (1 << 29),
	contents_ladder					= (1 << 29),
	contents_hitbox					= (1 << 30),
	contents_last_visible			= contents_opaque,
	contents_all_visible			= (contents_last_visible | (contents_last_visible - 1))
};

enum game_trace_masks : int
{
	mask_solid						= (contents_solid | contents_moveable | contents_window | contents_monster | contents_grate),
	mask_player_solid				= (contents_solid | contents_moveable | contents_player_clip | contents_window | contents_monster | contents_grate),
	mask_npc_solid					= (contents_solid | contents_moveable | contents_monster_clip | contents_window | contents_monster | contents_grate),
	mask_npc_fluid					= (contents_solid | contents_moveable | contents_monster_clip | contents_window | contents_monster),
	mask_water						= (contents_water | contents_moveable | contents_slime),
	mask_opaque						= (contents_solid | contents_moveable | contents_opaque),
	mask_opaque_and_npcs			= (mask_opaque | contents_monster),
	mask_blocklos					= (contents_solid | contents_moveable | contents_blocklos),
	mask_blocklos_and_npcs			= (mask_blocklos | contents_monster),
	mask_visible					= (mask_opaque | contents_ignore_nodraw_opaque),
	mask_visible_and_npcs			= (mask_opaque_and_npcs | contents_ignore_nodraw_opaque),
	mask_shot						= (contents_solid | contents_moveable | contents_monster | contents_window | contents_debris | contents_hitbox),
	mask_shot_brush_only			= (contents_solid | contents_moveable | contents_window | contents_debris),
	mask_shot_hull					= (contents_solid | contents_moveable | contents_monster | contents_window | contents_debris | contents_grate),
	mask_shot_portal				= (contents_solid | contents_moveable | contents_window | contents_monster),
	mask_solid_brush_only			= (contents_solid | contents_moveable | contents_window | contents_grate),
	mask_player_solid_brush_only	= (contents_solid | contents_moveable | contents_window | contents_player_clip | contents_grate),
	mask_npc_solid_brush_only		= (contents_solid | contents_moveable | contents_window | contents_monster_clip | contents_grate),
	mask_npc_world_static			= (contents_solid | contents_window | contents_monster_clip | contents_grate),
	mask_npc_world_static_fluid		= (contents_solid | contents_window | contents_monster_clip),
	mask_split_area_portal			= (contents_water | contents_slime),
	mask_current					= (contents_current_0 | contents_current_90 | contents_current_180 | contents_current_270 | contents_current_up | contents_current_down),
	mask_dead_solid					= (contents_solid | contents_player_clip | contents_window | contents_grate),
	mask_all						= (contents_solid | contents_window | contents_aux | contents_grate | contents_slime | contents_water | contents_blocklos | contents_opaque | contents_testfogvolum | contents_unused | contents_block_light | contents_team1 | contents_team2 | contents_ignore_nodraw_opaque | contents_moveable | contents_area_portal | contents_player_clip | contents_monster_clip | contents_current_0 | contents_current_90 | contents_current_180 | contents_current_270 | contents_current_up | contents_current_down | contents_origin | contents_monster | contents_debris | contents_detail | contents_translucent | contents_ladder | contents_hitbox)
};

enum game_surface_flags : int
{
	surf_light		= (1 << 0),
	surf_sky2d		= (1 << 1),
	surf_sky		= (1 << 2),
	surf_warp		= (1 << 3),
	surf_trans		= (1 << 4),
	surf_no_portal	= (1 << 5),
	surf_trigger	= (1 << 6),
	surf_no_draw	= (1 << 7),
	surf_hint		= (1 << 8),
	surf_skip		= (1 << 9),
	surf_no_light	= (1 << 10),
	surf_bump_light = (1 << 11),
	surf_no_shadows = (1 << 12),
	surf_no_decals	= (1 << 13),
	surf_no_chop	= (1 << 14),
	surf_hitbox		= (1 << 15)
};

enum game_displacement_surface : int
{
	displacement_surface		= (1 << 0),
	displacement_walkable		= (1 << 1),
	displacement_buildable		= (1 << 2),
	displacement_surface_prop1	= (1 << 3),
	displacement_surface_prop2	= (1 << 4)
};

#pragma endregion

struct game_brush_side_info
{
	vector_aligned	plane;		// the plane of the brush side
	std::uint16_t	bevel;		// bevel plane?
	std::uint16_t	thin;		// thin?
};

struct game_cplane
{
	vector_3d		normal;		// 0x00
	float			distance;	// 0x0c
	std::uint8_t	type;		// 0x10
	std::uint8_t	sign_bits;	// 0x11
};

struct game_csurface
{
	const char*		name;			// 0x00
	short			surface_props;	// 0x04
	std::uint16_t	flags;			// 0x06
};

class game_base_trace
{
public:
	game_base_trace() { }

	vector_3d		start;			// start position
	vector_3d		end;			// final position
	game_cplane		plane;			// surface normal at impact
	float			fraction;		// time completed, 1.0 = didn't hit anything
	int				contents;		// contents on other side of surface hit
	std::uint16_t	displacement;	// displacement flags for marking surfaces with data
	bool			all_solid;		// if true, plane is not valid
	bool			start_solid;	// if true, the initial point was in a solid area
};

class base_entity;
class game_trace : public game_base_trace
{
public:
	game_trace() : hit_entity(nullptr) { }

	float				fraction_left_solid;	// time we left a solid, only valid if we started in solid
	game_csurface		surface;				// surface hit (impact surface)
	int					hitgroup;				// 0 == generic, non-zero is specific body part
	short				physics_bone;			// physics bone hit by trace in studio
	std::uint16_t		world_surface_index;	// index of the msurface2_t, if applicable
	base_entity*		hit_entity;				// entity hit by trace
	int					hitbox;					// box hit by trace in studio

	inline bool did_hit() const { return (fraction < 1.f || all_solid || start_solid); }
	inline bool is_visible() const { return (fraction > 0.97f); }

private:
	game_trace(const game_trace& other)
	{
		this->start					= other.start;
		this->end					= other.end;
		this->plane					= other.plane;
		this->fraction				= other.fraction;
		this->contents				= other.contents;
		this->displacement			= other.displacement;
		this->all_solid				= other.all_solid;
		this->start_solid			= other.start_solid;
		this->fraction_left_solid	= other.fraction_left_solid;
		this->surface				= other.surface;
		this->hitgroup				= other.hitgroup;
		this->physics_bone			= other.physics_bone;
		this->world_surface_index	= other.world_surface_index;
		this->hit_entity			= other.hit_entity;
		this->hitbox				= other.hitbox;
	}
};

struct game_ray
{
	game_ray() : world_axis_transform{ }, is_ray{ }, is_swept{ } { }

	game_ray(const vector_3d& ray_start, const vector_3d& ray_end)
	{
		delta					= vector_aligned(ray_end - ray_start);
		is_swept				= (delta.length_sqr() != 0.f);
		world_axis_transform	= nullptr;
		is_ray					= true;
		start					= ray_start;

		start_offset.zero();
		extents.zero();
	}

	game_ray(const vector_3d& ray_start, const vector_3d& ray_end, const vector_3d& ray_mins, const vector_3d& ray_maxs)
	{
		delta					= vector_aligned(ray_end - ray_start);
		world_axis_transform	= nullptr;
		is_swept				= (delta.length_sqr() != 0.f);
		extents					= vector_aligned(ray_maxs - ray_mins);
		extents					*= 0.5f;
		is_ray					= (extents.length_sqr() < 1e-6);
		start_offset			= vector_aligned(ray_mins + ray_maxs);
		start_offset			*= 0.5f;
		start					= vector_aligned(ray_start + start_offset);
		start_offset			*= -1.f;
	}

	vector_aligned      start;
	vector_aligned      delta;
	vector_aligned		start_offset;
	vector_aligned		extents;
	const matrix_3x4*	world_axis_transform;
	bool				is_ray;
	bool				is_swept;
};

class handle_entity;
class base_trace_filter
{
public:
	virtual bool				should_hit_entity(handle_entity* entity, int contents_mask) = 0;
	virtual game_trace_type		trace_types() const = 0;
};

class trace_filter : public base_trace_filter
{
public:
	game_trace_type trace_types() const override
	{
		return trace_everything;
	}
};

class trace_filter_entities_only : public base_trace_filter
{
public:
	game_trace_type trace_types() const override
	{
		return trace_entities_only;
	}
};

class trace_filter_world_only : public base_trace_filter
{
public:
	bool should_hit_entity(handle_entity* entity, int contents_mask) override
	{
		return false;
	}

	game_trace_type trace_types() const override
	{
		return trace_world_only;
	}
};

class trace_filter_world_and_props_only : public base_trace_filter
{
public:
	bool should_hit_entity(handle_entity* entity, int contents_mask) override
	{
		return false;
	}

	game_trace_type trace_types() const override
	{
		return trace_everything;
	}
};

class game_trace_filter : public trace_filter
{
	using should_hit_callback = bool(__cdecl*)(handle_entity*, int);

public:
	game_trace_filter(const handle_entity* entity, int collision_group = collision_none, should_hit_callback should_hit = nullptr)
		: skip(entity), collision(collision_group), callback(should_hit) {
	}

	bool should_hit_entity(handle_entity* entity, int contents_mask) override
	{
		return entity != this->skip;
	}

	inline const handle_entity* skip_entity() const
	{
		return this->skip;
	}

	inline int collision_group() const
	{
		return this->collision;
	}

private:
	const handle_entity*	skip;		// 0x00
	int						collision;	// 0x04
	should_hit_callback		callback;	// 0x08
};

class trace_filter_skip_two_entities : public game_trace_filter
{
public:
	trace_filter_skip_two_entities(const handle_entity* first, const handle_entity* second, int collision_group = collision_none)
		: game_trace_filter(first, collision_group), skip2(second) {
	}

	bool should_hit_entity(handle_entity* entity, int contents_mask) override
	{
		return entity != this->skip2;
	}

private:
	const handle_entity* skip2;
};

class collideable;
class game_engine_trace
{
public:
	int		point_contents(const vector_3d& abs_position, int contents_mask = mask_all, handle_entity** entity = nullptr) { return virtuals->call<int>(this, 0, &abs_position, contents_mask, entity); }
	int		point_contents_world_only(const vector_3d& abs_position, int contents_mask = mask_all) { return virtuals->call<int>(this, 1, this, &abs_position, contents_mask); }
	int		point_contents_collideable(collideable* collide, const vector_3d& abs_position) { return virtuals->call<int>(this, 2, collide, &abs_position); }

	void	clip_ray_to_entity(const game_ray& ray, unsigned int mask, handle_entity* entity, game_trace* trace) { return virtuals->call<void>(this, 3, this, &ray, mask, entity, trace); }
	void	clip_ray_to_collideable(const game_ray& ray, unsigned int mask, collideable* collide, game_trace* trace) { return virtuals->call<void>(this, 4, &ray, mask, collide, trace); }
	void	trace_ray(const game_ray& ray, unsigned int mask, game_trace_filter* filter, game_trace* trace) { return virtuals->call<void>(this, 5, &ray, mask, filter, trace); }

	bool	brush_info(int brush, utl_vector<game_brush_side_info>* info, int* contents) { return virtuals->call<bool>(this, 18, brush, info, contents); }
};
