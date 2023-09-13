#pragma once
#include "../classes/clients.h"

class base_entity : public client_entity
{
public:
	NETVAR("CBaseEntity->m_iTeamNum", team, int)

	NETVAR("CBaseEntity->m_flAnimTime", animation_time, float)
	NETVAR("CBaseEntity->m_flSimulationTime", simulation_time, float)

	NETVAR_OFFSET("CBaseEntity->m_flSimulationTime", 0x4, old_simulation_time, float)

	NETVAR("CBaseEntity->m_bSpotted", spotted, bool)

	NETVAR("CBaseEntity->m_angRotation", rotation, q_angle)

	NETVAR("CBaseCombatCharacter->m_hActiveWeapon", active_weapon_handle, base_handle)

	DATAMAP(this->description_map(), "m_angAbsRotation", abs_rotation, q_angle)
	DATAMAP(this->description_map(), "m_rgflCoordinateFrame", coordinate_frame, const matrix_3x4)
	DATAMAP(this->description_map(), "m_MoveType", move_type, int)
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

class base_player : public base_entity
{
public:
	NETVAR("CBasePlayer->m_fFlags", flags, int)
	NETVAR("CBasePlayer->m_iHealth", health, int)
	NETVAR("CBasePlayer->m_nTickBase", tick_base, int)

	NETVAR_PTR("CBasePlayer->m_iObserverMode", observer_mode, int)

	NETVAR("CBasePlayer->m_aimPunchAngle", aim_punch, q_angle)
	NETVAR("CBasePlayer->m_viewPunchAngle", view_punch, q_angle)

	NETVAR_OFFSET_PTR("CBasePlayer->deadflag", 0x4, view_angles, q_angle)

	NETVAR("CBasePlayer->m_vecVelocity[0]", velocity, vector_3d)

	DATAMAP(this->prediction_map(), "m_iEFlags", eflags, int)

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