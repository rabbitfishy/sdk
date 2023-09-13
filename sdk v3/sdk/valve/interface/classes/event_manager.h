#pragma once

#define event_debug_id_setup		42
#define event_debug_id_shutdown		13

class game_event
{
public:
	virtual					~game_event() { }
	virtual const char*		name() const = 0;

	virtual bool			reliable() const = 0;
	virtual bool			local() const = 0;
	virtual bool			empty(const char* key_name = nullptr) = 0;

	virtual bool			get_bool(const char* key_name = nullptr, bool default_value = false) = 0;
	virtual int				get_int(const char* key_name = nullptr, int default_value = 0) = 0;
	virtual std::uint64_t	get_uint64(const char* key_name = nullptr, std::uint64_t default_value = 0) = 0;
	virtual float			get_float(const char* key_name = nullptr, float default_value = 0.f) = 0;
	virtual const char*		get_string(const char* key_name = nullptr, const char* default_value = "\0") = 0;
	virtual const wchar_t*	get_wstring(const char* key_name = nullptr, const wchar_t* default_value = L"\0") = 0;
	virtual const void*		get_ptr(const char* key_name = nullptr) const = 0;

	virtual void			set(const char* key_name, bool value) = 0;
	virtual void			set(const char* key_name, int value) = 0;
	virtual void			set(const char* key_name, std::uint64_t value) = 0;
	virtual void			set(const char* key_name, float value) = 0;
	virtual void			set(const char* key_name, const char* value) = 0;
	virtual void			set(const char* key_name, const wchar_t* value) = 0;
	virtual void			set(const char* key_name, const void* value) = 0;

	virtual bool			for_event_data(void* pevent) const = 0;
};

class game_event_listener
{
public:
	virtual			~game_event_listener() { }
	virtual void	fire_game_event(game_event* event) = 0;
	virtual int		get_event_debug_id() { return debug_id; }
public:
	int	debug_id;
};

class csvc_message_game_event;
class game_event_manager
{
public:
	virtual						~game_event_manager() { }
	virtual int					load_from_file(const char* file_name) = 0;
	virtual void				reset() = 0;
	virtual bool				add(game_event_listener* listener, const char* name, bool server_side) = 0;
	virtual bool				find(game_event_listener* listener, const char* name) = 0;
	virtual void				remove(game_event_listener* listener) = 0;
	virtual void				add_global(game_event_listener* listener, bool server_side) = 0;
	virtual game_event*			create_event(const char* name, bool force = false, int* unknown = nullptr) = 0;
	virtual bool				fire_event(game_event* event, bool dont_broadcast = false) = 0;
	virtual bool				fire_event_client_side(game_event* event) = 0;
	virtual game_event*			duplicate(game_event* event) = 0;
	virtual void				free(game_event* event) = 0;
	virtual bool				serialize_event(game_event* event, csvc_message_game_event* event_message) = 0;
	virtual game_event*			unserialize_event(const csvc_message_game_event& event_message) = 0;
	virtual game_key_value*		data_types(game_event* event) = 0;
};