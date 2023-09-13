#pragma once
#include <cstddef>
#include <cstdint>

#pragma region studio flags

enum game_hitgroup : int
{
	hitgroup_generic = 0,
	hitgroup_head,
	hitgroup_chest,
	hitgroup_stomach,
	hitgroup_left_arm,
	hitgroup_right_arm,
	hitgroup_left_leg,
	hitgroup_right_leg,
	hitgroup_neck,
	hitgroup_gear = 10
};

enum game_hitbox : int
{
	hitbox_invalid = -1,
	hitbox_head,
	hitbox_neck,
	hitbox_pelvis,
	hitbox_stomach,
	hitbox_thorax,
	hitbox_chest,
	hitbox_upper_chest,
	hitbox_right_thigh,
	hitbox_left_thigh,
	hitbox_right_calf,
	hitbox_left_calf,
	hitbox_right_foot,
	hitbox_left_foot,
	hitbox_right_hand,
	hitbox_left_hand,
	hitbox_right_upper_arm,
	hitbox_right_fore_arm,
	hitbox_left_upper_arm,
	hitbox_left_fore_arm,
	hitbox_max
};

#pragma endregion

#pragma region studio definitions

#define max_studio_bone_ctrls		4
#define max_studio_pose_param		24
#define max_studio_skins			32		// total textures
#define max_studio_flex_ctrl		96		// maximum number of flexcontrollers (input sliders)
#define max_studio_bones			128		// total bones actually used
#define max_studio_anim_blocks		256
#define max_studio_flex_desc		1024	// maximum number of low level flexes (actual morph targets)

#define bone_calculate_mask			0x1f
#define bone_physically_simulated	0x01
#define bone_physics_procedural		0x02
#define bone_always_procedural		0x04
#define bone_screen_align_sphere	0x08
#define bone_screen_align_cylinder	0x10

#define bone_used_mask				0x0007ff00
#define bone_used_by_anything		0x0007ff00
#define bone_used_by_hitbox			0x00000100
#define bone_used_by_attachment		0x00000200
#define bone_used_by_vertex_mask	0x0003fc00
#define bone_used_by_vertex_lod0	0x00000400
#define bone_used_by_vertex_lod1	0x00000800
#define bone_used_by_vertex_lod2	0x00001000
#define bone_used_by_vertex_lod3	0x00002000
#define bone_used_by_vertex_lod4	0x00004000
#define bone_used_by_vertex_lod5	0x00008000
#define bone_used_by_vertex_lod6	0x00010000
#define bone_used_by_vertex_lod7	0x00020000
#define bone_used_by_bone_merge		0x00040000
#define bone_always_setup			0x00080000

#define bone_used_by_vertex_at_lod( lod ) ( bone_used_by_vertex_lod0 << ( lod ) )
#define bone_used_by_anything_at_lod( lod ) ( ( bone_used_by_anything & ~bone_used_by_vertex_mask ) | bone_used_by_vertex_at_lod( lod ) )

#define max_lods_count				8

#define bone_type_mask				0x00f00000
#define bone_fixed_alignment		0x00100000
#define bone_has_save_frame_pos		0x00200000
#define bone_has_save_frame_rot64	0x00400000
#define bone_has_save_frame_rot32	0x00800000

#pragma endregion

struct game_studio_bone
{
	int			name_index;
	int			parent;
	std::byte	_pad0[0x98];
	int			flags;
	std::byte	_pad1[0x34];

	const char* name() const noexcept { return name_index ? reinterpret_cast<const char*>(std::uintptr_t(this) + name_index) : nullptr; }
};

struct game_studio_bounding_box
{
	int			bone;
	int			group;					// intersection group
	vector_3d	mins;					// bounding box
	vector_3d	maxs;
	int			hitbox_name_index;		// offset to the name of the hitbox
	q_angle		offset_orientation;
	float		radius;
	int			unused[4];
};

struct game_studio_hitbox_set
{
	int name_index;
	int hitbox_count;
	int hitbox_index;

	const char* name() const noexcept { return reinterpret_cast<char*>(std::uintptr_t(this) + name_index); }

	game_studio_bounding_box* hitbox(int index) noexcept
	{
		if (index < 0 || index >= hitbox_count) return nullptr;
		return reinterpret_cast<game_studio_bounding_box*>(reinterpret_cast<std::uint8_t*>(this) + hitbox_index) + index;
	}
};

class game_studio_hdr
{
public:
	int			id;
	int			version;
	int			checksum;
	char		name[64];
	int			length;

	vector_3d	eye_position;
	vector_3d	illum_position;
	vector_3d	hull_min;
	vector_3d	hull_max;
	vector_3d	mins;
	vector_3d	maxs;

	int			flags;
	int			bone_count;
	int			bone_index;
	int			bone_controller_count;
	int			bone_controller_index;
	int			hitbox_set_count;
	int			hitbox_set_index;

	game_studio_bone* bone(int index) noexcept
	{
		if (index < 0 || index >= bone_count) return nullptr;
		return reinterpret_cast<game_studio_bone*>(std::uintptr_t(this) + bone_index) + index;
	}

	game_studio_hitbox_set* hitbox_set(int index) noexcept
	{
		if (index < 0 || index >= hitbox_set_count) return nullptr;
		return reinterpret_cast<game_studio_hitbox_set*>(std::uintptr_t(this) + hitbox_set_index) + index;
	}
};