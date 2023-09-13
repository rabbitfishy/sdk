#pragma once

struct game_surface_physics_params
{
	float friction;
	float elasticity;
	float density;
	float thickness;
	float dampening;
}; // Size: 0x14

struct game_surface_audio_params
{
	float reflectivity;					// like elasticity, but how much sound should be reflected by this surface.
	float hardness_factor;				// like elasticity, but only affects impact sound choices.
	float roughness_factor;				// like friction, but only affects scrape sound choices.
	float rough_threshold;				// surface roughness > this causes "rough" scrapes, < this causes "smooth" scrapes.
	float hard_threshold;				// surface hardness > this causes "hard" impacts, < this causes "soft" impacts.
	float hard_velocity_threshold;		// collision velocity > this causes "hard" impacts, < this causes "soft" impacts.
	float high_pitch_occlusion;			// a value betweeen 0 and 100 where 0 is not occluded at all and 100 is silent (except for any additional reflected sound).
	float mid_pitch_occlusion;
	float low_pitch_occlusion;
}; // Size: 0x24

struct game_surface_sound_names
{
	std::uint16_t walk_step_left;
	std::uint16_t walk_step_right;
	std::uint16_t run_step_left;
	std::uint16_t run_step_right;
	std::uint16_t impact_soft;
	std::uint16_t impact_hard;
	std::uint16_t scrape_smooth;
	std::uint16_t scrape_rough;
	std::uint16_t bullet_impact;
	std::uint16_t rolling;
	std::uint16_t break_sound;
	std::uint16_t strain_sound;
}; // Size: 0x18

struct game_surface_sound_handles
{
	std::uint16_t walk_step_left;
	std::uint16_t walk_step_right;
	std::uint16_t run_step_left;
	std::uint16_t run_step_right;
	std::uint16_t impact_soft;
	std::uint16_t impact_hard;
	std::uint16_t scrape_smooth;
	std::uint16_t scrape_rough;
	std::uint16_t bullet_impact;
	std::uint16_t rolling;
	std::uint16_t break_sound;
	std::uint16_t strain_sound;
}; // Size: 0x18

struct surface_game_props
{
	float					max_speed_factor;
	float					jump_factor;
	float					penetration_modifier;
	float					damage_modifier;
	std::uint16_t			material;
	std::byte				climbable;
	std::byte				_pad0[0x4];
}; // Size: 0x17

struct game_surface_data
{
	game_surface_physics_params		physics;
	game_surface_audio_params		audio;
	game_surface_sound_names		sounds;
	surface_game_props				game;
	game_surface_sound_handles		sound_handles;
}; // Size: 0x7F

class game_physics_surface_props
{
public:
	virtual						~game_physics_surface_props() { }
	virtual int					parse_surface_data(const char* file_name, const char* text_file) = 0;
	virtual int					surface_prop_count() const = 0;
	virtual int					surface_index(const char* surface_prop_name) const = 0;
	virtual void				physics_properties(int surface_data_index, float* density, float* thickness, float* friction, float* elasticity) const = 0;
	virtual game_surface_data*	surface_data(int surface_data_index) = 0;
	virtual const char*			string(unsigned short string_table_index) const = 0;
	virtual const char*			prop_name(int surface_data_index) const = 0;
	virtual void				set_world_material_index_table(int* map_array, int map_size) = 0;
	virtual void				physics_parameters(int surface_data_index, game_surface_physics_params* params_out) const = 0;
};