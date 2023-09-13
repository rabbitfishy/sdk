#pragma once

#pragma region engine sound flags

enum game_sound_channel
{
	chan_replace	= -1,
	chan_auto		= 0,
	chan_weapon		= 1,
	chan_voice		= 2,
	chan_item		= 3,
	chan_body		= 4,
	chan_stream		= 5,						// allocate stream channel from the static or dynamic area
	chan_static		= 6,						// allocate channel from the static area 
	chan_voice2		= 7,
	chan_voice_base = 8,						// allocate channel for network voice data
	chan_user_base	= (chan_voice_base + 128)	// anything >= this number is allocated to game code.
};

enum game_sound_level : int
{
	snd_lvl_none		= 0,
	snd_lvl_20db		= 20,			// rustling leaves
	snd_lvl_25db		= 25,			// whispering
	snd_lvl_30db		= 30,			// library
	snd_lvl_35db		= 35,
	snd_lvl_40db		= 40,
	snd_lvl_45db		= 45,			// refrigerator
	snd_lvl_50db		= 50,			// 3.9	// average home
	snd_lvl_55db		= 55,			// 3.0

	snd_lvl_idle		= 60,			// 2.0	
	snd_lvl_60db		= 60,			// 2.0	// normal conversation, clothes dryer
	snd_lvl_65db		= 65,			// 1.5	// washing machine, dishwasher

	snd_lvl_static		= 66,			// 1.25
	snd_lvl_70db		= 70,			// 1.0	// car, vacuum cleaner, mixer, electric sewing machine

	snd_lvl_norm		= 75,
	snd_lvl_75db		= 75,			// 0.8	// busy traffic
	snd_lvl_80db		= 80,			// 0.7	// mini-bike, alarm clock, noisy restaurant, office tabulator, outboard motor, passing snowmobile

	snd_lvl_talking		= 80,			// 0.7
	snd_lvl_85db		= 85,			// 0.6	// average factory, electric shaver
	snd_lvl_90db		= 90,			// 0.5	// screaming child, passing motorcycle, convertible ride on frw
	snd_lvl_95db		= 95,
	snd_lvl_100db		= 100,			// 0.4	// subway train, diesel truck, woodworking shop, pneumatic drill, boiler shop, jackhammer
	snd_lvl_105db		= 105,			// helicopter, power mower
	snd_lvl_110db		= 110,			// snowmobile drvrs seat, inboard motorboat, sandblasting
	snd_lvl_120db		= 120,			// auto horn, propeller aircraft
	snd_lvl_130db		= 130,			// air raid siren

	snd_lvl_gunfire		= 140,			// 0.27	// threshold of pain, gunshot, jet engine
	snd_lvl_140db		= 140,			// 0.2
	snd_lvl_150db		= 150,			// 0.2
	snd_lvl_180db		= 180,			// rocket launching
};

enum game_sound_flags : int
{
	snd_no_flags								= 0,			// to keep the compiler happy
	snd_change_vol								= (1 << 0),		// change sound vol
	snd_change_pitch							= (1 << 1),		// change sound pitch
	snd_stop									= (1 << 2),		// stop the sound
	snd_spawning								= (1 << 3),		// we're spawning, used in some cases for ambients
	// not sent over net, only a param between dll and server.
	snd_delay									= (1 << 4),		// sound has an initial delay
	snd_stop_looping							= (1 << 5),		// stop all looping sounds on the entity.
	snd_speaker									= (1 << 6),		// being played again by a microphone through a speaker
	snd_should_pause							= (1 << 7),		// this sound should be paused if the game is paused
	snd_ignore_phonemes							= (1 << 8),
	snd_ignore_name								= (1 << 9),		// used to change all sounds emitted by an entity, regardless of scriptname
	snd_do_not_overwrite_existing_on_channel	= (1 << 10)
};

#pragma endregion

#pragma region engine sound definitions

// volume 
#define vol_norm			1.0f

// attenuation 
#define attn_none			0.0f
#define attn_norm			0.8f
#define attn_idle			2.0f
#define attn_static			1.25f 
#define attn_ricochet		1.5f
#define attn_gunfire		0.27f
#define max_attenuation		3.98f

// sound level limits 
#define max_sndlvl_bits		9
#define min_sndlvl_value	0
#define max_sndlvl_value	((1 << max_sndlvl_bits) - 1)

// sound level 
#define sndlevel_to_compatibility_mode( x )		static_cast<game_sound_level>(static_cast<int>(x + 256))
#define sndlevel_from_compatibility_mode( x )	static_cast<game_sound_level>(static_cast<int>(x - 256))
#define sndlevel_is_compatibility_mode( x )		( x ) >= 256

// convertation 
#define attn_to_sndlvl( a ) static_cast<game_sound_level>(static_cast<int>( a ) ? (50 + 20 / (static_cast<float>( a ))) : 0)
#define sndlvl_to_attn( a ) ((a > 50) ? (20.0f / static_cast<float>(a - 50)) : 4.0)

// sound 
#define snd_flag_bits_encode 11

#define max_sound_delay_msec_encode_bits	13
#define max_sound_index_bits				14
#define	max_sounds							(1 << max_sound_index_bits)
#define max_sound_delay_msec				(1 << (max_sound_delay_msec_encode_bits - 1))    // 4096 msec or about 4 seconds

// source 
#define sound_from_ui_panel			-2
#define sound_from_local_player		-1
#define sound_from_world			0

// pitch 
#define pitch_low			95
#define	pitch_norm			100
#define pitch_high			120

#define default_sound_packet_volume 1.0f
#define default_sound_packet_pitch	100
#define default_sound_packet_delay	0.0f

#pragma endregion

using game_file_name_handle = void*;

struct game_sound_info
{
	int						guid;
	game_file_name_handle	file_name;
	int						sound_source;
	int						channel;
	int						speaker_entity;
	float					volume;
	float					last_spatialized_volume;
	float					radius;
	int						pitch;
	vector_3d*				origin;
	vector_3d*				direction;
	bool					update_positions;
	bool					is_sentence;
	bool					dry_mix;
	bool					speaker;
	bool					special_dsp;
	bool					from_server;
};

class sfx_table;

struct start_sound_parameters
{
	bool				static_sound;
	int					user_data;
	int					sound_source;
	int					entity_channel;
	sfx_table*			sfx;
	vector_3d			origin;
	vector_3d			direction;
	bool				update_positions;
	float				volume;
	game_sound_level	sound_level;
	int					flags;
	int					pitch;
	bool				from_server;
	float				delay;
	int					speaker_entity;
	bool				suppress_recording;
	int					initial_stream_position;
};

class game_recipient_filter
{
public:
	virtual			~game_recipient_filter() { }

	virtual bool	is_reliable() const = 0;
	virtual bool	is_init_message() const = 0;

	virtual int		recipient_count() const = 0;
	virtual int		recipient_index(int nslot) const = 0;
};

class vector_3d;

class game_engine_sound
{
public:
	virtual bool	pre_cache_sound(const char* sample, bool preload = false, bool is_ui_sound = false) = 0;
	virtual bool	is_sound_pre_cached(const char* sample) = 0;
	virtual void	pre_fetch_sound(const char* sample) = 0;
	virtual bool	is_looping_sound(const char* sample) = 0;
	virtual float	sound_duration(const char* sample) = 0;

	virtual int		emit_sound(game_recipient_filter& filter, int entity_index, int channel, const char* sound_entry, unsigned int sound_entry_hash, const char* sample,
		float volume, float attenuation, int seed, int flags, int pitch,
		const vector_3d* origin, const vector_3d* direction, utl_vector<vector_3d>* utl_origins, bool update_positions, float sound_time, int speaker_entity, start_sound_parameters& parameters) = 0;

	virtual int		emit_sound(game_recipient_filter& filter, int entity_index, int channel, const char* sound_entry, unsigned int sound_entry_hash, const char* sample,
		float volume, game_sound_level sound_level, int seed, int flags, int pitch,
		const vector_3d* origin, const vector_3d* direction, utl_vector<vector_3d>* utl_origins, bool update_positions, float sound_time, int speaker_entity, start_sound_parameters& parameters) = 0;

	virtual void	emit_sentence_by_index(game_recipient_filter& filter, int entity_index, int channel, int sentence_index,
		float volume, game_sound_level sound_level, int seed, int flags = 0, int pitch = pitch_norm,
		const vector_3d* origin = nullptr, const vector_3d* direction = nullptr, utl_vector<vector_3d>* utl_origins = nullptr, bool update_positions = true, float sound_time = 0.0f, int speaker_entity = -1) = 0;

	virtual void	stop_sound(int entity_index, int channel, const char* sample, unsigned int sound_entry_hash) = 0;
	virtual void	stop_all_sounds(bool clear_buffers) = 0;
	virtual void	set_room_type(game_recipient_filter& filter, int room_type) = 0;
	virtual void	set_player_dsp(game_recipient_filter& filter, int dsp_type, bool fast_reset) = 0;
	virtual void	emit_ambient_sound(const char* sample, float volume, int pitch = pitch_norm, int flags = 0, float sound_time = 0.0f) = 0;
	virtual float	distance_gain_from_sound_level(game_sound_level sound_level, float distance) = 0;
	virtual int		guid_for_last_sound_emitted() = 0;
	virtual bool	is_sound_still_playing(int guid) = 0;
	virtual void	stop_sound_by_guid(int guid, bool force_sync) = 0;
	virtual void	set_volume_by_guid(int guid, float volume) = 0;
private:
	virtual void	function0() = 0;
public:
	virtual void	active_sounds(utl_vector<game_sound_info>& sound_list) = 0;
	virtual void	pre_cache_sentence_group(const char* group_name) = 0;
	virtual void	notify_begin_movie_playback() = 0;
	virtual void	notify_end_movie_playback() = 0;
	virtual bool	sound_channel_volume(const char* sound, float& volume_left, float& volume_right) = 0;
	virtual float	elapsed_time_by_guid(int guid) = 0;
	virtual bool	prevent_sound() = 0;
	virtual void	set_replay_sound_fade(float replay_volume) = 0;
	virtual float	replay_sound_fade() const = 0;
};