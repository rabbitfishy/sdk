#pragma once

#define invalid_string_table -1
#define invalid_string_index (std::uint16_t)~0

class game_network_table;
using network_string_changed = void(__cdecl*)(void*, game_network_table*, int, char const*, void const*);

class game_network_table
{
public:
	virtual					~game_network_table() { }

	// table info.
	virtual const char*		table_name() const = 0;
	virtual int				table_id() const = 0;
	virtual int				string_count() const = 0;
	virtual int				max_strings() const = 0;
	virtual int				entry_bits() const = 0;

	// networking.
	virtual void			set_tick(int tick) = 0;
	virtual bool			changed_since_tick(int tick) const = 0;

	// accessors.
	virtual int				add(bool is_server, const char* value, int length = -1, const void* user_data = 0) = 0;

	virtual const char*		get(int string) = 0;
	virtual void			set_string_user_data(int string, int length, const void* user_data) = 0;
	virtual const void*		get_string_user_data(int string, int* length) = 0;
	virtual int				find(char const* string) = 0;

	virtual void			set_string_changed_callback(void* object, network_string_changed change_func) = 0;
};

class game_network_container
{
public:
	game_network_table*		find(const char* table_name) { return virtuals->call<game_network_table*>(this, 3, table_name); }
};