#pragma once
#include "../classes/clients.h"
#include "../other/animations.h"

class base_entity : public game_client_entity
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

	NETVAR_OFFSET("CBaseAnimating->m_nForceBone", 0x4, most_recent_bone_counter, unsigned long)
	NETVAR_OFFSET("CBaseAnimating->m_nForceBone", 0x3C, bone_attachments, utl_vector<base_handle>)
	NETVAR_OFFSET("CBaseAnimating->m_nForceBone", 0x18, bone_accessor, game_bone_accessor)

	NETVAR_OFFSET("CBaseAnimating->m_hLightingOrigin", -0x20, last_bone_setup_time, float)
	NETVAR_OFFSET("CBaseAnimating->m_hLightingOrigin", -0x18, allow_jiggle_bones, bool)

	game_studio_hdr* model_ptr()
	{
		game_studio_hdr* hdr = interfaces->model_info->studio_model(this->get_model());

		if (!hdr)
			return nullptr;

		return hdr;
	}

	void update_client_side_animation()
	{
		virtuals->call<void>(this, 224);
	}

	void set_sequence(int handle)
	{
		virtuals->call<void>(this, 219, handle);
	}

	void studio_frame_advance()
	{
		virtuals->call<void>(this, 220);
	}

	void invalidate_bone_cache()
	{
		static unsigned long& model_bone_counter = *SEARCH(modules->client, signatures::entity::model_bone_counter::signature()).add(0x2).reinterpret<unsigned long*>();
		this->most_recent_bone_counter() = model_bone_counter - 1;
		this->last_bone_setup_time() = -FLT_MAX;
	}

	int bone_by_hash(const hash32 bone_hash);
	vector_3d bone_position(int bone_index);
	vector_3d hitbox_position(int hitbox_index);
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

class base_combat_weapon : public base_animating
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

	vector_3d eye_position()
	{
		vector_3d position = this->client_entity()->abs_origin() + this->view_offset();
		return position;
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

	NETVAR_OFFSET("CCSPlayer->m_flLastExoJumpTime", 0x8, is_new_animation_state, bool)

	NETVAR("CCSPlayer->m_angEyeAngles[0]", eye_angle, q_angle)

	NETVAR_OFFSET("CCSPlayer->m_bIsScoped", -0x14, animation_state, game_animation_state*)

	vector_3d shoot_position();
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
		return convars::other::inferno_flame_lifetime::convar()->get_float();
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


