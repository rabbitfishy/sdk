#pragma once
#include "../interface/classes/key_value_system.h"
#include <cstddef>
#include <cstdint>

using symbol_proc = bool(__cdecl*)(const char*);

class game_key_value
{
public:
	enum key_type : int
	{
		type_none = 0,
		type_string,
		type_int,
		type_float,
		type_ptr,
		type_wstring,
		type_color,
		type_uint64,
		type_compiled_int_byte,
		type_compiled_int_0,
		type_compiled_int_1,
		type_count
	};

	game_key_value(const char* key_name, void* unknown1 = nullptr, h_key_symbol case_insensitive_key_name = invalid_key_symbol);
	~game_key_value();

	void* operator new(std::size_t alloc_size);
	void operator delete(void* memory_ptr);

	const char* get_name();

	static game_key_value* from_string(const char* name, const char* value);
	void load_from_buffer(char const* resource_name, const char* buffer, void* file_system = nullptr, const char* path_id = nullptr, symbol_proc evaluate_symbol_proc = nullptr);
	bool load_from_file(void* file_system, const char* resource_name, const char* path_id = nullptr, symbol_proc evaluate_symbol_proc = nullptr);

	game_key_value* find(const char* key_name, const bool create);

	int get_int(const char* key_name, const int default_value);
	float get_float(const char* key_name, const float default_value);
	const char* get_string(const char* key_name, const char* default_value);

	void set_string(const char* key_name, const char* string_value);
	void set_int(const char* key_name, const int value);
	void set_uint64(const char* key_name, const int low_value, const int high_value);

	inline void set_bool(const char* key_name, const bool value) { set_int(key_name, value ? 1 : 0); }

private:
	std::uint32_t		key_name : 24;						// 0x00
	std::uint32_t		key_name_case_sensitive1 : 8;		// 0x3 // byte, explicitly specify bits due to packing
	char*				value;								// 0x04
	wchar_t*			wide_value;							// 0x08

	union
	{
		int				int_value;
		float			float_value;
		void*			value_ptr;
		unsigned char	color[4];
	}; // 0x0c

	std::int8_t			data_type;							// 0x10
	bool				has_escape_sequences;				// 0x11
	std::uint16_t		key_name_case_sensitive2;			// 0x12
	void*				unknown14;							// 0x14 // seems like ikeyvaluessystem*, but why do they need it here? also calling smth on destructor and cleans up
	bool				has_case_insensitive_symbol;		// 0x18
	game_key_value*		peer;								// 0x1c
	game_key_value*		sub;								// 0x20
	game_key_value*		chain;								// 0x24
	symbol_proc			expression_symbol_proc;				// 0x28
};