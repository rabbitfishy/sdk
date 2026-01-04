#pragma once
#include "client_mode.h"
#include <cstdint>

using mdl_handle = std::uint16_t;

#pragma region mdl cache flags

enum
{
	mdl_handle_invalid = static_cast<mdl_handle>(~0)
};

enum game_mdl_cache_data_type : int
{
	mdl_cache_studio_hdr = 0,
	mdl_cache_studio_hwdata,
	mdl_cache_vcollide,
	mdl_cache_anim_block,
	mdl_cache_virtual_model,
	mdl_cache_vertexes,
	mdl_cache_decoded_anim_block
};

enum game_mdl_cache_flush_flags : unsigned int
{
	mdl_cache_flush_studio_hdr		= (1 << 0),
	mdl_cache_flush_studio_hwdata	= (1 << 1),
	mdl_cache_flush_vcollide		= (1 << 2),
	mdl_cache_flush_anim_block		= (1 << 3),
	mdl_cache_flush_virtual_model	= (1 << 4),
	mdl_cache_flush_autoplay		= (1 << 5),
	mdl_cache_flush_vertexes		= (1 << 6),
	mdl_cache_flush_combined_data	= (1 << 7),
	mdl_cache_flush_ignore_lock		= (1 << 31),
	mdl_cache_flush_all				= 0xFFFFFFFF
};

#pragma endregion

class game_mdl_cache_notify
{
public:
	virtual void on_data_loaded(game_mdl_cache_data_type type, mdl_handle model)		= 0;
	virtual void on_combiner_pre_cache(mdl_handle old_handle, mdl_handle new_handle)	= 0;
	virtual void on_data_unloaded(game_mdl_cache_data_type type, mdl_handle model)		= 0;
	virtual bool should_supress_load_warning(mdl_handle model)							= 0;
};

class game_mdl_cache : public game_app_system
{
public:
	void set_cache_notify(game_mdl_cache_notify* notify)												{ virtuals->call<void>(this, 9, notify); }
	[[nodiscard]] mdl_handle find_mdl(const char* relative_path)										{ return virtuals->call<mdl_handle>(this, 10, relative_path); }
	[[nodiscard]] int add_reference(mdl_handle model)													{ return virtuals->call<int>(this, 11, model); }
	[[nodiscard]] int release(mdl_handle model)															{ return virtuals->call<int>(this, 12, model); }
	[[nodiscard]] int get_reference(mdl_handle model)													{ return virtuals->call<int>(this, 13, model); }
	[[nodiscard]] game_studio_hdr* get_studio_hdr(mdl_handle model)										{ return virtuals->call<game_studio_hdr*>(this, 14, model); }
	[[nodiscard]] unsigned char* get_anim_block(mdl_handle model, int block, bool preload_if_missing)	{ return virtuals->call<unsigned char*>(this, 18, model, block, preload_if_missing); }
	[[nodiscard]] bool has_anim_block_been_preloaded(mdl_handle model, int block)						{ return virtuals->call<bool>(this, 19, model, block); }
	[[nodiscard]] int get_autoplay_list(mdl_handle model, unsigned short** out)							{ return virtuals->call<int>(this, 21, model, out); }
	[[nodiscard]] bool is_error_model(mdl_handle model)													{ return virtuals->call<bool>(this, 26, model); }
	void flush(game_mdl_cache_flush_flags flush_flags = mdl_cache_flush_all)							{ virtuals->call<void>(this, 28, flush_flags); }
	void flush(mdl_handle model, game_mdl_cache_flush_flags flush_flags = mdl_cache_flush_all)			{ virtuals->call<void>(this, 29, model, flush_flags); }
	[[nodiscard]] const char* get_model_name(mdl_handle model)											{ return virtuals->call<const char*>(this, 30, model); }
	[[nodiscard]] void* get_cache_section(game_mdl_cache_data_type type)								{ return virtuals->call<void*>(this, 31, type); }
	void begin_lock()																					{ virtuals->call<void>(this, 33); }
	void end_lock()																						{ virtuals->call<void>(this, 34); }
	[[nodiscard]] game_studio_hdr* lock_studio_hdr(mdl_handle model)									{ return virtuals->call<game_studio_hdr*>(this, 48, model); }
	void unlock_studio_hdr(mdl_handle model)															{ virtuals->call<void>(this, 49, model); }
	[[nodiscard]] bool preload_model(mdl_handle model)													{ return virtuals->call<bool>(this, 50, model); }
	void begin_coarse_lock()																			{ virtuals->call<void>(this, 53); }
	void end_coarse_lock()																				{ virtuals->call<void>(this, 54); }
	void reload_vcollide(mdl_handle model)																{ virtuals->call<void>(this, 55, model); }
	void disable_vcollide_load()																		{ virtuals->call<void>(this, 58); }
	void enable_vcollide_load()																			{ virtuals->call<void>(this, 59); }
};

class game_mdl_cache_critical_section
{
public:
	game_mdl_cache_critical_section(game_mdl_cache* handle) : cache(handle)
	{
		this->cache->begin_lock();
	}

	~game_mdl_cache_critical_section()
	{
		this->cache->end_lock();
	}

	// class no assignment.
	game_mdl_cache_critical_section& operator=(game_mdl_cache_critical_section&&)		= delete;
	game_mdl_cache_critical_section& operator=(const game_mdl_cache_critical_section&&) = delete;

private:
	game_mdl_cache* cache;
};