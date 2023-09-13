#pragma once
#include <functional>

#pragma region engine trace flags

enum game_displacement_surface : int
{
	displacement_surface		= (1 << 0),
	displacement_walkable		= (1 << 1),
	displacement_buildable		= (1 << 2),
	displacement_surface_prop1	= (1 << 3),
	displacement_surface_prop2	= (1 << 4)
};

enum game_trace_type : int
{
	trace_everything = 0,
	trace_world_only,
	trace_entities_only,
	trace_everything_filter_props,
	trace_filter_sky
};

enum debug_trace_counter_behavior : int
{
	trace_counter_set = 0,
	trace_counter_inc,
};

enum game_trace_contents : unsigned int
{
	contents_empty					= 0,
	contents_solid					= 0x1,
	contents_window					= 0x2,
	contents_aux					= 0x4,
	contents_grate					= 0x8,
	contents_slime					= 0x10,
	contents_water					= 0x20,
	contents_blocklos				= 0x40,
	contents_opaque					= 0x80,
	contents_test_fog_volume		= 0x100,
	contents_unused					= 0x200,
	contents_block_light			= 0x400,
	contents_team1					= 0x800,
	contents_team2					= 0x1000,
	contents_ignore_nodraw_opaque	= 0x2000,
	contents_moveable				= 0x4000,
	contents_area_portal			= 0x8000,
	contents_player_clip			= 0x10000,
	contents_monster_clip			= 0x20000,
	contents_current_0				= 0x40000,
	contents_current_90				= 0x80000,
	contents_current_180			= 0x100000,
	contents_current_270			= 0x200000,
	contents_current_up				= 0x400000,
	contents_current_down			= 0x800000,
	contents_origin					= 0x1000000,
	contents_monster				= 0x2000000,
	contents_debris					= 0x4000000,
	contents_detail					= 0x8000000,
	contents_translucent			= 0x10000000,
	contents_ladder					= 0x20000000,
	contents_hitbox					= 0x40000000
};

enum game_trace_masks : unsigned int
{
	mask_all					= 0xffffffff,
	mask_solid					= contents_solid | contents_moveable | contents_window | contents_monster | contents_grate,
	mask_player_solid			= contents_solid | contents_moveable | contents_player_clip | contents_window | contents_monster | contents_grate,
	mask_npc_solid				= contents_solid | contents_moveable | contents_monster_clip | contents_window | contents_monster | contents_grate,
	mask_npc_fluid				= contents_solid | contents_moveable | contents_monster_clip | contents_window | contents_monster | contents_grate,
	mask_water					= contents_water | contents_moveable | contents_slime,
	mask_opaque					= contents_solid | contents_moveable | contents_opaque,
	mask_opaque_and_npcs		= mask_opaque | contents_monster,
	mask_blocklos				= contents_solid | contents_moveable | contents_blocklos,
	mask_blocklos_and_npcs		= mask_blocklos | contents_monster,
	mask_visible				= mask_opaque | contents_ignore_nodraw_opaque,
	mask_visible_and_npcs		= mask_opaque_and_npcs | contents_ignore_nodraw_opaque,
	mask_shot					= contents_solid | contents_moveable | contents_monster | contents_window | contents_debris | contents_grate | contents_hitbox,
	mask_shot_brush_only		= contents_solid | contents_moveable | contents_window | contents_debris,
	mask_shot_hull				= contents_solid | contents_moveable | contents_monster | contents_window | contents_debris | contents_grate,
	mask_shot_portal			= contents_solid | contents_moveable | contents_window | contents_monster,
	mask_solid_brushonly		= contents_solid | contents_moveable | contents_window | contents_grate,
	mask_playersolid_brush_only = contents_solid | contents_moveable | contents_window | contents_player_clip | contents_grate,
	mask_npc_solid_brush_only	= contents_solid | contents_moveable | contents_window | contents_monster_clip | contents_grate,
	mask_npc_world_static		= contents_solid | contents_window | contents_monster_clip | contents_grate,
	mask_npc_world_static_fluid	= contents_solid | contents_window | contents_monster_clip,
	mask_split_area_portal		= contents_water | contents_slime,
	mask_current				= contents_current_0 | contents_current_90 | contents_current_180 | contents_current_270 | contents_current_up | contents_current_down,
	mask_dead_solid				= contents_solid | contents_player_clip | contents_window | contents_grate,
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
	vector_3d	normal;
	float		distance;
	std::byte	type;
	std::byte	sign_bits;
	std::byte	_pad0[0x2];
};

struct game_csurface
{
	const char*		name;
	short			surface_props;
	std::uint16_t	flags;
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

	inline bool did_hit() const { return (fraction < 1.0f || all_solid || start_solid); }
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
	game_ray() : world_axis_transform{ }, is_ray{ }, is_swept{ } { };

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
class game_base_trace_filter
{
public:
	virtual bool				should_hit_entity(handle_entity* entity, int contents_mask) = 0;
	virtual game_trace_type		trace_types() const = 0;
};

class game_trace_filter : public game_base_trace_filter
{
	using game_filter_callback = std::function<bool(handle_entity*, int)>;
public:
	game_trace_filter(const handle_entity* skip_entity, game_trace_type types = trace_everything)
		: skip(skip_entity), trace_type(types) { }

	game_trace_filter(game_filter_callback&& check_callback, game_trace_type types = trace_everything)
		: check_callback(std::move(check_callback)), trace_type(types) { }

	bool should_hit_entity(handle_entity* entity, int contents_mask) override
	{
		// if given user defined callback - check it.
		if (check_callback != nullptr)
			return check_callback(entity, contents_mask);

		// otherwise skip entity if given.
		return entity != skip;
	}

	game_trace_type trace_types() const override { return trace_type; }

private:
	const handle_entity* skip				= nullptr;
	game_filter_callback check_callback		= nullptr;
	game_trace_type trace_type				= trace_everything;
};

class game_trace_list_data
{
public:
	virtual			~game_trace_list_data() { }
	virtual void	reset() = 0;
	virtual bool	is_empty() = 0;
	virtual bool	can_trace_ray(const game_ray& ray) = 0;
};

class game_entity_enumerator
{
public:
	// this gets called with each handle.
	virtual bool enumerate_entity(handle_entity* entity) = 0;
};

class collideable;
class game_engine_trace
{
public:
	virtual int		point_contents(const vector_3d& abs_position, int contents_mask = mask_all, handle_entity** entity = nullptr) = 0;
	virtual int		point_contents_world_only(const vector_3d& abs_position, int contents_mask = mask_all) = 0;
	virtual int		point_contents_collideable(collideable* collide, const vector_3d& abs_position) = 0;
	virtual void	clip_ray_to_entity(const game_ray& ray, unsigned int mask, handle_entity* entity, game_trace* trace) = 0;
	virtual void	clip_ray_to_collideable(const game_ray& ray, unsigned int mask, collideable* collide, game_trace* trace) = 0;
	virtual void	trace_ray(const game_ray& ray, unsigned int mask, game_trace_filter* trace_filter, game_trace* trace) = 0;
};