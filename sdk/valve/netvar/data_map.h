#pragma once
#include <cstddef>
#include <cstdint>

#pragma region data map flags

enum field_types : int
{
	field_void = 0,					// no type or value
	field_float,					// any floating point value
	field_string,					// a string id (return from alloc_string)
	field_vector,					// any vector, qangle, or angularimpulse
	field_quaternion,				// a quaternion
	field_integer,					// any integer or enum
	field_boolean,					// boolean, implemented as an int, i may use this as a hint for compression
	field_short,					// 2 byte integer
	field_character,				// a byte
	field_color_32,					// 8-bit per channel r,g,b,a (32bit color)
	field_embedded,					// an embedded object with a datadesc, recursively traverse and embedded class/structure based on an additional typedescription
	field_custom,					// special type that contains function pointers to it's read/write/parse functions
	field_class_ptr,				// cbaseentity*
	field_entity_handle,			// entity handle
	field_edict,					// edict_t*
	field_position_vector,			// a world coordinate (these are fixed up across level transitions automagically)
	field_time,						// a floating point time (these are fixed up automatically too!)
	field_tick,						// an integer tick count( fixed up similarly to time)
	field_model_name,				// engine string that is a model name (needs precache)
	field_sound_name,				// engine string that is a sound name (needs precache)
	field_input,					// a list of inputed data fields (all derived from cmultiinputvar)
	field_function,					// a class function pointer (think, use, etc)
	field_vmatrix,					// a vmatrix (output coords are not worldspace)
	field_vmatrix_world_space,		// a vmatrix that maps some local space to world space (translation is fixed up on level transitions)
	field_matrix_3x4_world_space,	// matrix3x4_t that maps some local space to world space (translation is fixed up on level transitions)
	field_interval,					// a start and range floating point interval ( e.g., 3.2->3.6 == 3.2 and 0.4 )
	field_model_index,				// a model index
	field_material_index,			// a material index (using the material precache string table)
	field_vector_2d,				// 2 floats
	field_integer_64,				// 64bit integer
	field_vector_4d,				// 4 floats
	field_type_count
};

enum
{
	data_offset_normal = 0,
	data_offset_packed = 1,
	data_offset_count
};

#pragma endregion

struct game_data_map;

struct game_type_description
{
public:
	field_types			type;							// 0x0000
	const char*			name;							// 0x0004
	int					offset[data_offset_count];		// 0x0008
	unsigned short		size;							// 0x0010
	short				flags;							// 0x0012
	std::byte			_pad0[0xc];						// 0x0014
	game_data_map*		description;					// 0x0020
	std::byte			_pad1[0x18];					// 0x0024
}; // size: 0x003c

struct game_data_map
{
	game_type_description*	descriptions;
	int						fields;
	const char*				name;
	game_data_map*			data_map;
	bool					chains_validated;
	bool					packed_offsets_computed;
	int						packed_size;
};