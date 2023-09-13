#pragma once
#include "../../dependencies/memory/virtual.h"
#include "../maths/utl_vector.h"
#include <cstddef>
#include <cstdint>

using command_callback = void(__cdecl*)();
using change_callback = void(__cdecl*)(void*, const char*, float);

class game_convar
{
public:

#pragma region convar get methods

	const char* get_name() { return virtuals->call<const char*>(this, 5); }

	float get_float()
	{
		std::uint32_t xored = *reinterpret_cast<std::uint32_t*>(&parent->float_value) ^ reinterpret_cast<std::uint32_t>(this);
		return *reinterpret_cast<float*>(&xored);
	}

	int get_int() { return static_cast<int>(parent->int_value ^ reinterpret_cast<int>(this)); }
	bool get_bool() { return !!get_int(); }

	const char* get_string() const
	{
		const auto value = parent->string; 
		return value ? value : "\0";
	}

#pragma endregion

#pragma region convar set methods

	void set(const char* value) { return virtuals->call<void>(this, 14, value); }
	void set(float value) { return virtuals->call<void>(this, 15, value); }
	void set(int value) { return virtuals->call<void>(this, 16, value); }
	//void set(color value) { return virtuals->call<void>(this, 17, value); }

#pragma endregion

#pragma region convar variables

	std::byte						_pad0[0x4];			// 0x00
	game_convar*					next;				// 0x04
	bool							registered;			// 0x08
	const char*						name;				// 0x0C
	const char*						help_string;		// 0x10
	int								flags;				// 0x14
	command_callback				callback;			// 0x18
	game_convar*					parent;				// 0x1C
	const char*						default_value;		// 0x20
	char*							string;				// 0x24
	int								string_length;		// 0x28
	float							float_value;		// 0x2C
	int								int_value;			// 0x30
	bool							has_min;			// 0x34
	float							min_value;			// 0x38
	bool							has_max;			// 0x3C
	float							max_value;			// 0x40
	utl_vector<change_callback>		change_callbacks;	// 0x44

#pragma endregion
};

class game_spoofed_convar
{
public:
	game_spoofed_convar() = default;
	game_spoofed_convar(const char* var);
	game_spoofed_convar(game_convar* var);
	~game_spoofed_convar();

	// checks
	bool	is_spoofed() const;
	void	spoof();

	// flags
	void	set_flags(int flags) const;
	int		get_flags() const;

	// methods
	void	set(bool value) const;
	void	set(int value) const;
	void	set(float value) const;
	void	set(const char* value) const;

private:
	game_convar*	original_convar = nullptr;
	char			original_name[128];
	char			original_value[128];
	int				original_flags = 0;

	game_convar*	dummy_convar = nullptr;
	char			dummy_name[128];
	char			dummy_value[128];
};