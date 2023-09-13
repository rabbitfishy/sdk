#pragma once
#include "../interface/interface.h"
#include "../netvar/netvar.h"
#include "../other/bitbuf.h"
#include "../../dependencies/memory/modules.h"
#include "../../dependencies/memory/signatures.h"

class collideable;
class client_networkable;
class client_renderable;
class client_entity;
class base_entity;
class client_thinkable;
class client_alpha_property;

class client_unknown : public handle_entity
{
public:
	virtual collideable*			collideable() = 0;
	virtual client_networkable*		networkable() = 0;
	virtual client_renderable*		renderable() = 0;
	virtual client_entity*			client_entity() = 0;
	virtual base_entity*			entity() = 0;
	virtual client_thinkable*		thinkable() = 0;
	virtual client_alpha_property*	alpha_property() = 0;
};

#pragma region collideable

enum solid_type
{
	solid_none = 0,	// no solid model
	solid_bsp = 1,	// a bsp tree
	solid_bbox = 2,	// an aabb
	solid_obb = 3,	// an obb (not implemented yet)
	solid_obb_yaw = 4,	// an obb, constrained so that it can only yaw
	solid_custom = 5,	// always call into the entity for tests
	solid_vphysics = 6,	// solid vphysics object, get vcollide from the model and collide with that
	solid_last,
};

class collideable
{
public:
	virtual handle_entity*			entity_handle() = 0;
	virtual const vector_3d&		obb_mins() const = 0;
	virtual const vector_3d&		obb_maxs() const = 0;
	virtual void					world_space_trigger_bounds(vector_3d* mins, vector_3d* maxs) const = 0;
	virtual bool					test_collision(const game_ray& ray, unsigned int mask, game_trace& tr) = 0;
	virtual bool					test_hitboxes(const game_ray& ray, unsigned int mask, game_trace& tr) = 0;
	virtual int						collision_model_index() = 0;
	virtual const game_model*		collision_model() = 0;
	virtual const vector_3d&		collision_origin() const = 0;
	virtual const q_angle&			collision_angles() const = 0;
	virtual const matrix_3x4&		collision_to_world_transform() const = 0;
	virtual solid_type				get_solid() const = 0;
	virtual int						get_solid_flags() const = 0;
	virtual client_unknown*			unknown() = 0;
	virtual int						get_collision_group() const = 0;

	/*
	* note: should be equivalent to C_BaseAnimating::ComputeHitboxSurroundingBox
	* DOESNT NEEDED TRANSORMATION!
	* and bugged when trying to get non-player entity box
	*/
	virtual void					world_space_surrounding_bounds(vector_3d* mins, vector_3d* maxs) = 0;
	virtual unsigned int			get_required_trigger_flags() const = 0;
	// returns null unless this collideable has specified fsolid_root_parent_aligned
	virtual const matrix_3x4*		get_root_parent_to_world_transform() const = 0;
	virtual void*					get_vphysics_object() const = 0;
};

#pragma endregion

#pragma region networkable

enum should_transmit_state
{
	should_transmit_start = 0,
	should_transmit_end
};

enum data_update_type
{
	data_update_created = 0,
	data_update_datatable_changed,
	data_update_post_update,
};

class client_networkable
{
public:
	virtual client_unknown*			unknown() = 0;
	virtual void					release() = 0;
	virtual game_client_class*		client_class() = 0;
	virtual void					notify_should_transmit(should_transmit_state state) = 0;
	virtual void					on_pre_data_changed(data_update_type update_type) = 0;
	virtual void					on_data_changed(data_update_type update_type) = 0;
	virtual void					pre_data_update(data_update_type update_type) = 0;
	virtual void					post_data_update(data_update_type update_type) = 0;
	virtual void					on_data_unchanged_in_pvs() = 0;
	virtual bool					dormant(void) const = 0;
	virtual int						index(void) const = 0;
	virtual void					receive_message(int class_id, bf_read& message) = 0;
	virtual void*					datatable_base_ptr() = 0;
	virtual void					set_destroyed_on_recreate_entities(void) = 0;
};

#pragma endregion

#pragma region renderable

enum model_data_category
{
	model_data_lighting_model,	// data type returned is a renderablelightingmodel_t
	model_data_stencil,			// data type returned is a shaderstencilstate_t
	model_data_category_count,
};

enum shadow_type
{
	shadows_none = 0,
	shadows_simple,
	shadows_render_to_texture,
	shadows_render_to_texture_dynamic,			// the shadow is always changing state
	shadows_render_to_depth_texture,
	shadows_render_to_texture_dynamic_custom,	// changing, and entity uses custom rendering code for shadow
};

class client_model_renderable
{
public:
	virtual bool						get_render_data(void* data, model_data_category category) = 0;
};

struct renderable_instance
{
	std::uint8_t alpha;
};

class client_renderable
{
public:
	virtual client_unknown*				unknown() = 0;
	virtual vector_3d const&			render_origin(void) = 0;
	virtual q_angle const&				render_angles(void) = 0;
	virtual bool						should_draw(void) = 0;
	virtual int							render_flags(void) = 0;
	virtual void						unused(void) const { }
	virtual unsigned short				shadow_handle() const = 0;
	virtual unsigned short&				render_handle() = 0;
	virtual const game_model*			get_model() const = 0;
	virtual int							draw_model(int flags, const renderable_instance& instance) = 0;
	virtual int							get_body() = 0;
	virtual void						color_modulation(float* color) = 0;
	virtual bool						lod_test() = 0;
	virtual bool						setup_bones(matrix_3x4* bone_to_world_out, int max_bones, int bone_mask, float current_time) = 0;
	virtual void						setup_weights(const matrix_3x4* bone_to_world, int flex_weight_count, float* flex_weights, float* flex_delayed_weights) = 0;
	virtual void						do_animation_events(void) = 0;
	virtual void*						pvs_notify_interface() = 0;
	virtual void						render_bounds(vector_3d& mins, vector_3d& maxs) = 0;
	virtual void						render_bounds_world_space(vector_3d& mins, vector_3d& maxs) = 0;
	virtual void						shadow_render_bounds(vector_3d& mins, vector_3d& maxs, shadow_type type) = 0;
	virtual bool						should_receive_projected_textures(int flags) = 0;
	virtual bool						shadow_cast_distance(float* dist, shadow_type type) const = 0;
	virtual bool						shadow_cast_direction(vector_3d* direction, shadow_type type) const = 0;
	virtual bool						is_shadow_dirty() = 0;
	virtual void						mark_shadow_dirty(bool dirty) = 0;
	virtual client_renderable*			get_shadow_parent() = 0;
	virtual client_renderable*			first_shadow_child() = 0;
	virtual client_renderable*			next_shadow_peer() = 0;
	virtual shadow_type					shadow_cast_type() = 0;
	virtual void						unused2() { }
	virtual void						create_model_instance() = 0;
	virtual unsigned short				model_instance() = 0;
	virtual const matrix_3x4&			renderable_to_world_transform() = 0;
	virtual int							lookup_attachment(const char* attachment_name) = 0;
	virtual	bool						get_attachment(int number, vector_3d& origin, vector_3d& angles) = 0;
	virtual bool						get_attachment(int number, matrix_3x4& matrix) = 0;
	virtual bool						compute_lighting_origin(int attachment_index, vector_3d model_lighting_center, const matrix_3x4& matrix, vector_3d& transformed_lighting_center) = 0;
	virtual float*						render_clip_plane(void) = 0;
	virtual int							get_skin() = 0;
	virtual void						on_threaded_draw_setup() = 0;
	virtual bool						uses_flex_delayed_weights() = 0;
	virtual void						record_tool_message() = 0;
	virtual bool						should_draw_for_split_screen_user(int slot) = 0;
	virtual std::uint8_t				override_alpha_modulation(std::uint8_t alpha) = 0;
	virtual std::uint8_t				override_shadow_alpha_modulation(std::uint8_t alpha) = 0;
	virtual client_model_renderable*	client_model_renderable() = 0;
};

#pragma endregion

#pragma region thinkable

class client_thinkable
{
public:
	virtual client_unknown*		unknown() = 0;
	virtual void				client_think() = 0;
	virtual void*				get_think_handle() = 0;
	virtual void				set_think_handle(int think) = 0;
	virtual void				release() = 0;
};

#pragma endregion

#pragma region alpha_property

enum client_alpha_distance_fade_mode
{
	client_alpha_distance_fade_use_center = 0,
	client_alpha_distance_fade_use_nearest_bbox,
	client_alpha_distance_fade_mode_count,
};

enum render_fx
{
	render_fx_none = 0,
	render_fx_pulse_slow,
	render_fx_pulse_fast,
	render_fx_pulse_slow_wide,
	render_fx_pulse_fast_wide,

	render_fx_fade_slow,
	render_fx_fade_fast,
	render_fx_solid_slow,
	render_fx_solid_fast,
	render_fx_strobe_slow,

	render_fx_strobe_fast,
	render_fx_strobe_faster,
	render_fx_flicker_slow,
	render_fx_flicker_fast,
	render_fx_no_dissipation,

	render_fx_fade_out,
	render_fx_fade_in,
	render_fx_pulse_fast_wider,
	render_fx_glow_shell,

	render_fx_max
};

enum render_mode
{
	render_normal = 0,				// src
	render_trans_color,				// c*a+dest*(1-a)
	render_trans_texture,			// src*a+dest*(1-a)
	render_glow,					// src*a+dest -- no z buffer checks -- fixed size in screen space
	render_trans_alpha,				// src*srca+dest*(1-srca)
	render_trans_add,				// src*a+dest
	render_environmental,			// not drawn, used for environmental effects
	render_trans_add_frame_blend,	// use a fractional frame value to blend between animation frames
	render_trans_alpha_add,			// src + dest*(1-a)
	render_world_glow,				// same as krenderglow but not fixed size in screen space
	render_none,					// don't render.

	render_mode_count,				// must be last
};

class client_alpha_property
{
public:
	virtual client_unknown*		unknown() = 0;
	virtual void				set_alpha_modulation(std::uint8_t alpha) = 0;
	virtual void				set_render_fx(render_fx render_fx, render_mode render_mode, float start_time = std::numeric_limits<float>::max(), float duration = 0.0f) = 0;
	virtual void				set_fade(float global_fade_scale, float dist_fade_start, float dist_fade_end) = 0;
	virtual void				set_desync_offset(int offset) = 0;
	virtual void				enable_alpha_modulation_override(bool enable) = 0;
	virtual void				enable_shadow_alpha_modulation_override(bool enable) = 0;
	virtual void				set_distance_fade_mode(client_alpha_distance_fade_mode fade_mode) = 0;
};

#pragma endregion

#pragma region client_entity

class mouth_info;

class client_entity : public client_unknown, public client_renderable, public client_networkable, public client_thinkable
{
public:
	virtual void				release(void) = 0;
	virtual const vector_3d&	abs_origin(void) const = 0;
	virtual const q_angle&		abs_angles(void) const = 0;
	virtual mouth_info*			get_mouth(void) = 0;
	virtual bool				sound_spatialization(struct spatialization_info& info) = 0;
	virtual bool				is_blurred(void) = 0;

	void set_abs_origin(const vector_3d& origin)
	{
		using set_abs_origin_details = void(__thiscall*)(void*, const vector_3d&);
		static auto original_set_abs_origin = SEARCH(modules->client, signatures::entity::set_abs_origin::signature()).cast<set_abs_origin_details>();
		original_set_abs_origin(this, origin);
	}

	void set_abs_angle(const q_angle& view_angle)
	{
		using set_abs_angle_details = void(__thiscall*)(void*, const q_angle&);
		static auto original_set_abs_angle = SEARCH(modules->client, signatures::entity::set_abs_angle::signature()).cast<set_abs_angle_details>();
		original_set_abs_angle(this, view_angle);
	}

	game_data_map* description_map() { return virtuals->call<game_data_map*>(this, 15); }
	game_data_map* prediction_map() { return virtuals->call<game_data_map*>(this, 17); }
};

#pragma endregion