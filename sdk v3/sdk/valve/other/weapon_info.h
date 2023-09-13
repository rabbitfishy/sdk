#pragma once
#include <cstddef>
#include <cstdint>

#pragma region weapon entity flags

enum item_definition_index : short
{
	weapon_none									= 0,
	weapon_deagle								= 1,
	weapon_elite								= 2,
	weapon_fiveseven							= 3,
	weapon_glock								= 4,
	weapon_ak47									= 7,
	weapon_aug									= 8,
	weapon_awp									= 9,
	weapon_famas								= 10,
	weapon_g3sg1								= 11,
	weapon_galilar								= 13,
	weapon_m249									= 14,
	weapon_m4a1									= 16,
	weapon_mac10								= 17,
	weapon_p90									= 19,
	weapon_zone_repulsor						= 20,
	weapon_mp5sd								= 23,
	weapon_ump45								= 24,
	weapon_xm1014								= 25,
	weapon_bizon								= 26,
	weapon_mag7									= 27,
	weapon_negev								= 28,
	weapon_sawedoff								= 29,
	weapon_tec9									= 30,
	weapon_taser								= 31,
	weapon_hkp2000								= 32,
	weapon_mp7									= 33,
	weapon_mp9									= 34,
	weapon_nova									= 35,
	weapon_p250									= 36,
	weapon_shield								= 37,
	weapon_scar20								= 38,
	weapon_sg556								= 39,
	weapon_ssg08								= 40,
	weapon_knife_gg								= 41,
	weapon_knife								= 42,
	weapon_flashbang							= 43,
	weapon_he_grenade							= 44,
	weapon_smoke_grenade						= 45,
	weapon_molotov								= 46,
	weapon_decoy								= 47,
	weapon_incgrenade							= 48,
	weapon_c4									= 49,
	weapon_healthshot							= 57,
	weapon_knife_t								= 59,
	weapon_m4a1_silencer						= 60,
	weapon_usp_silencer							= 61,
	weapon_cz75a								= 63,
	weapon_revolver								= 64,
	weapon_tagrenade							= 68,
	weapon_fists								= 69,
	weapon_breach_charge						= 70,
	weapon_tablet								= 72,
	weapon_melee								= 74,
	weapon_axe									= 75,
	weapon_hammer								= 76,
	weapon_spanner								= 78,
	weapon_knife_ghost							= 80,
	weapon_fire_bomb							= 81,
	weapon_diversion							= 82,
	weapon_frag_grenade							= 83,
	weapon_snowball								= 84,
	weapon_bump_mine							= 85,
	weapon_knife_bayonet						= 500,
	weapon_knife_css							= 503,
	weapon_knife_flip							= 505,
	weapon_knife_gut							= 506,
	weapon_knife_karambit						= 507,
	weapon_knife_m9_bayonet						= 508,
	weapon_knife_tactical						= 509,
	weapon_knife_falchion						= 512,
	weapon_knife_survival_bowie					= 514,
	weapon_knife_butterfly						= 515,
	weapon_knife_push							= 516,
	weapon_knife_cord							= 517,
	weapon_knife_canis							= 518,
	weapon_knife_ursus							= 519,
	weapon_knife_gypsy_jack_knife				= 520,
	weapon_knife_outdoor						= 521,
	weapon_knife_stiletto						= 522,
	weapon_knife_widowmaker						= 523,
	weapon_knife_skeleton						= 525,
	glove_studded_broken_fang					= 4725,
	glove_studded_bloodhound					= 5027,
	glove_t										= 5028,
	glove_ct									= 5029,
	glove_sporty								= 5030,
	glove_slick									= 5031,
	glove_leather_handwraps						= 5032,
	glove_motorcycle							= 5033,
	glove_specialist							= 5034,
	glove_studded_hydra							= 5035,
	special_agent_blueberries_buckshot			= 4619,
	special_agent_two_times_mccoy_tacp			= 4680,
	special_agent_commandor_mae_jamison			= 4711,
	special_agent_1st_lieutenant_farlow,
	special_agent_john_kask,
	special_agent_bio_haz_specialist,
	special_agent_sergeant_bombson,
	special_agent_chem_haz_specialist,
	special_agent_rezan_the_redshirt			= 4718,
	special_agent_sir_bloody_miami_darryl		= 4726,
	special_agent_safecracker_voltzmann,
	special_agent_little_kev,
	special_agent_getaway_sally					= 4730,
	special_agent_number_k						= 4732,
	special_agent_sir_bloody_silent_darryl		= 4733,
	special_agent_sir_bloody_skullhead_darryl,
	special_agent_sir_bloody_darryl_royale,
	special_agent_sir_bloody_loudmouth_darryl,
	special_agent_t								= 5036,
	special_agent_ct							= 5037,
	special_agent_ground_rebel					= 5105,
	special_agent_osiris,
	special_agent_shahmat,
	special_agent_muhlik,
	special_agent_soldier						= 5205,
	special_agent_enforcer,
	special_agent_slingshot,
	special_agent_street_soldier,
	special_agent_operator						= 5305,
	special_agent_markus_delrow,
	special_agent_michael_syfers,
	special_agent_ava,
	special_agent_3rd_commando_company			= 5400,
	special_agent_seal_team_6_soldier,
	special_agent_buckshot,
	special_agent_two_times_mccoy_usaf,
	special_agent_ricksaw,
	special_agent_dragomir						= 5500,
	special_agent_maximus,
	special_agent_rezan_the_ready,
	special_agent_blackwolf						= 5503,
	special_agent_the_doctor,
	special_agent_dragomir_foot_soldiers,
	special_agent_b_squadron_officer			= 5601
};

enum weapon_type : int
{
	weapon_type_knife			= 0,
	weapon_type_pistol			= 1,
	weapon_type_smg				= 2,
	weapon_type_rifle			= 3,
	weapon_type_shotgun			= 4,
	weapon_type_sniper			= 5,
	weapon_type_heavy			= 6,
	weapon_type_c4				= 7,
	weapon_type_place_holder	= 8,
	weapon_type_grenade			= 9,
	weapon_type_healthshot		= 11,
	weapon_type_fists			= 12,
	weapon_type_breach_charge	= 13,
	weapon_type_bump_mine		= 14,
	weapon_type_tablet			= 15,
	weapon_type_melee			= 16
};

#pragma endregion

class game_weapon_info
{
public:
	std::byte		_pad0[0x14];					// 0x0000
	int				max_clip1;						// 0x0014
	int				max_clip2;						// 0x0018
	int				default_clip1;					// 0x001c
	int				default_clip2;					// 0x0020
	int				primary_max_reserve_ammo;		// 0x0024
	int				secondary_max_reserve_ammo;		// 0x0028
	const char*		world_model;					// 0x002c
	const char*		view_model;						// 0x0030
	const char*		dropped_model;					// 0x0034
	std::byte		_pad1[0x50];					// 0x0038
	const char*		hud_name;						// 0x0088
	const char*		weapon_name;					// 0x008c
	std::byte		_pad2[0x2];						// 0x0090
	bool			is_melee_weapon;				// 0x0092
	std::byte		_pad3[0x9];						// 0x0093
	float			weapon_weight;					// 0x009c
	std::byte		_pad4[0x4];						// 0x00a0
	int				slot;							// 0x00a4
	int				position;						// 0x00a8
	std::byte		_pad5[0x1c];					// 0x00ac
	int				weapon_type;					// 0x00c8
	std::byte		_pad6[0x4];						// 0x00cc
	int				weapon_price;					// 0x00d0
	int				kill_award;						// 0x00d4
	const char*		animation_prefix;				// 0x00d8
	float			cycle_time;						// 0x00dc
	float			cycle_time_alt;					// 0x00e0
	std::byte		_pad8[0x8];						// 0x00e4
	bool			full_auto;						// 0x00ec
	std::byte		_pad9[0x3];						// 0x00ed
	int				damage;							// 0x00f0
	float			headshot_multiplier;			// 0x00f4
	float			armor_ratio;					// 0x00f8
	int				bullets;						// 0x00fc
	float			penetration;					// 0x0100
	std::byte		_pad10[0x8];					// 0x0104
	float			range;							// 0x010c
	float			range_modifier;					// 0x0110
	float			throw_velocity;					// 0x0114
	std::byte		_pad11[0xc];					// 0x0118
	bool			has_silencer;					// 0x0124
	std::byte		_pad12[0xf];					// 0x0125
	float			max_speed[2];					// 0x0134
	std::byte		_pad13[0x4];					// 0x013c
	float			spread[2];						// 0x0140
	float			inaccuracy_crouch[2];			// 0x0148
	float			inaccuracy_stand[2];			// 0x0150
	std::byte		_pad14[0x8];					// 0x0158
	float			inaccuracy_jump[2];				// 0x0160
	float			inaccuracy_land[2];				// 0x0168
	float			inaccuracy_ladder[2];			// 0x0170
	float			inaccuracy_fire[2];				// 0x0178
	float			inaccuracy_move[2];				// 0x0180
	float			inaccuracy_reload;				// 0x0188
	int				recoil_seed;					// 0x018c
	float			recoil_angle[2];				// 0x0190
	float			recoil_angle_variance[2];		// 0x0198
	float			recoil_magnitude[2];			// 0x01a0
	float			recoil_magnitude_variance[2];	// 0x01a8
	int				spread_seed;					// 0x01b0
};