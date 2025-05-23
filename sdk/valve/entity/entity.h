#pragma once
#include "../classes/clients.h"
#include "../other/animations.h"

class base_entity : public client_entity
{
public:
	NETVAR("CBaseEntity->m_iTeamNum", team, int)

	NETVAR("CBaseEntity->m_flAnimTime", animation_time, float)
	NETVAR("CBaseEntity->m_flSimulationTime", simulation_time, float)

	NETVAR_OFFSET("CBaseEntity->m_flSimulationTime", 0x4, old_simulation_time, float)

	NETVAR("CBaseEntity->m_bSpotted", spotted, bool)

	NETVAR("CBaseEntity->m_vecOrigin", origin, vector_3d)

	NETVAR("CBaseEntity->m_angRotation", rotation, q_angle)

	NETVAR("CBaseCombatCharacter->m_hActiveWeapon", active_weapon_handle, base_handle)

	DATAMAP(this->prediction_map(), "m_iEFlags", eflags, int)
	DATAMAP(this->prediction_map(), "m_MoveType", move_type, int)

	DATAMAP(this->prediction_map(), "m_vecAbsVelocity", abs_velocity, vector_3d)
	DATAMAP(this->prediction_map(), "m_angAbsRotation", abs_rotation, q_angle)

	DATAMAP(this->description_map(), "m_rgflCoordinateFrame", coordinate_frame, const matrix_3x4&)
};

class base_animating : public base_entity
{
public:
	NETVAR("CBaseAnimating->m_nHitboxSet", hitbox_set, int)
	NETVAR("CBaseAnimating->m_nSequence", sequence, int)

	NETVAR("CBaseAnimating->m_flCycle", cycle, float)

	NETVAR("CBaseAnimating->m_bClientSideAnimation", client_side_animation, bool)

	NETVAR_OFFSET("CBaseAnimating->m_nForceBone", 0x4, most_recent_model_bone_counter, unsigned long)
	NETVAR_OFFSET("CBaseAnimating->m_nForceBone", 0x3C, bone_attachments, utl_vector<base_handle>)
	NETVAR_OFFSET("CBaseAnimating->m_nForceBone", 0x18, bone_accessor, game_bone_accessor)

	NETVAR_OFFSET("CBaseAnimating->m_hLightingOrigin", -0x20, last_bone_setup_time, float)
	NETVAR_OFFSET("CBaseAnimating->m_hLightingOrigin", -0x18, allow_jiggle_bones, bool)
	NETVAR_OFFSET("CBaseAnimating->m_hLightingOrigin", 0x8, studio_hdr, game_studio_hdr*)
		
	static unsigned long& model_bone_counter()
	{
		static auto original_model_bone_counter = SEARCH(modules->client, signatures::entity::model_bone_counter::signature()).add<std::uint8_t*>(0x2);
		return *reinterpret_cast<unsigned long*>(original_model_bone_counter);
	}

	void lock_studio_hdr()
	{
		static auto original_lock_studio_hdr = SEARCH(modules->client, signatures::entity::lock_studio_hdr::signature()).cast<void(__thiscall*)(base_animating*)>();
		original_lock_studio_hdr(this);
	}

	game_studio_hdr* model_ptr()
	{
		if (!this->studio_hdr() && this->get_model() != nullptr)
			this->lock_studio_hdr();

		game_studio_hdr* hdr = this->studio_hdr();
		if (hdr != nullptr)
			return hdr;

		return nullptr;
	}

	bool bone_cache_valid() { return (this->most_recent_model_bone_counter() == this->model_bone_counter()); }

	int bone_by_hash(const hash32 bone_hash);
	vector_3d bone_position(int bone_index);
};

class base_animating_overlay : public base_animating
{
public:
	utl_vector<game_animation_layer>& animation_overlays()
	{
		static const std::uintptr_t original_animation_overlays = *SEARCH(modules->client, signatures::entity::animation_overlays::signature()).add<std::uintptr_t*>(0x2);
		return *reinterpret_cast<utl_vector<game_animation_layer>*>(reinterpret_cast<std::uint8_t*>(this) + original_animation_overlays);
	}

	bool update_dispatch_layer(game_animation_layer* layer, game_studio_hdr* hdr, int sequence) { return virtuals->call<bool>(this, 247, layer, hdr, sequence); }
};

class base_combat_weapon : public client_entity
{
public:
	NETVAR("CBaseCombatWeapon->m_flNextPrimaryAttack", next_primary_attack, float)
	NETVAR("CBaseCombatWeapon->m_flNextSecondaryAttack", next_secondary_attack, float)

	NETVAR("CBaseAttributableItem->m_iItemDefinitionIndex", item_definition_index, short)

	NETVAR("CBaseCombatWeapon->m_iClip1", ammo, int)

	[[nodiscard]] float spread() { return virtuals->call<float>(this, 453); }
	[[nodiscard]] float inaccuracy() { return virtuals->call<float>(this, 483); }

	game_weapon_info* weapon_data();
};

class base_player : public base_animating_overlay
{
public:
	NETVAR("CBasePlayer->m_fFlags", flags, int)
	NETVAR("CBasePlayer->m_iHealth", health, int)
	NETVAR("CBasePlayer->m_nTickBase", tick_base, int)
	NETVAR("CBasePlayer->m_iObserverMode", observer_mode, int)

	NETVAR("CBasePlayer->m_aimPunchAngle", aim_punch, q_angle)
	NETVAR("CBasePlayer->m_viewPunchAngle", view_punch, q_angle)

	NETVAR_OFFSET_PTR("CBasePlayer->deadflag", 0x4, view_angles, q_angle)

	NETVAR("CBasePlayer->m_vecVelocity[0]", velocity, vector_3d)
	NETVAR("CBasePlayer->m_vecViewOffset[0]", view_offset, vector_3d)

	NETVAR("CBasePlayer->m_hGroundEntity", ground_entity_handle, base_handle)

	base_combat_weapon* active_weapon();

	bool alive()
	{
		if (!this) return false;
		return this->health() > 0;
	}
};

class cs_player : public base_player
{
public:
	NETVAR("CCSPlayer->m_ArmorValue", armor, int)

	NETVAR("CCSPlayer->m_flLowerBodyYawTarget", lby, float)
	NETVAR("CCSPlayer->m_flDuckAmount", duck_amount, float)
	NETVAR("CCSPlayer->m_flFlashDuration", flash_duration, float)
	NETVAR("CCSPlayer->m_flFlashMaxAlpha", flash_max_alpha, float)

	NETVAR("CCSPlayer->m_bIsScoped", scoped, bool)
	NETVAR("CCSPlayer->m_bHasHelmet", helmet, bool)
	NETVAR("CCSPlayer->m_bGunGameImmunity", immunity, bool)
	NETVAR("CCSPlayer->m_bHasHeavyArmor", heavy_armor, bool)
};

class weapon_cs_base : public base_combat_weapon
{
public:
	NETVAR("CWeaponCSBase->m_fAccuracyPenalty", accuracy_penalty, float)
	NETVAR("CWeaponCSBase->m_flPostponeFireReadyTime", fire_ready_time, float)
};

class base_cs_grenade : public weapon_cs_base
{
public:
	NETVAR("CBaseCSGrenade->m_bPinPulled", pin_pulled, bool)
	NETVAR("CBaseCSGrenade->m_fThrowTime", throw_time, float)
	NETVAR("CBaseCSGrenade->m_flThrowStrength", throw_strength, float)
	NETVAR("CBaseCSGrenadeProjectile->m_nExplodeEffectTickBegin", effect_tick_begin, int)
};

class smoke_grenade
{
public:
	NETVAR("CSmokeGrenadeProjectile->m_nSmokeEffectTickBegin", effect_tick_begin, int)
	float expiry_time() { return 18.f; }
};

class inferno
{
public:
	NETVAR("CInferno->m_nFireEffectTickBegin", effect_tick_begin, int)

	float expiry_time() {
		static game_convar* inferno_flame_lifetime = interfaces->convar->find("inferno_flame_lifetime");
		return inferno_flame_lifetime->get_float();
	}
};

class planted_c4
{
public:
	NETVAR("CPlantedC4->m_bBombTicking", planted, bool)
	NETVAR("CPlantedC4->m_bBombDefused", defused, bool)

	NETVAR("CPlantedC4->m_flC4Blow", blow_time, float)
	NETVAR("CPlantedC4->m_flTimerLength", bomb_timer_length, float)
	NETVAR("CPlantedC4->m_flDefuseLength", defuse_length, float)
	NETVAR("CPlantedC4->m_flDefuseCountDown", defuse_countdown, float)

	NETVAR("CPlantedC4->m_hBombDefuser", defuser_handle, base_handle)

	float bomb_timer(const float server_time) { return std::clamp(this->blow_time() - server_time, 0.f, this->bomb_timer_length()); }
	float defuse_timer(const float server_time) { return std::clamp(this->defuse_countdown() - server_time, 0.f, this->defuse_length()); }
};
