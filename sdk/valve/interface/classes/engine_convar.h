#pragma once

#pragma region engine convar flags

enum game_engine_convar_flags
{
	// convar systems
	cvar_none						= 0,
	cvar_unregistered				= (1 << 0),		// if this is set, don't add to linked list, etc.
	cvar_development_only			= (1 << 1),		// hidden in released products. flag is removed automatically if allow_development_cvars is defined.
	cvar_game_dll					= (1 << 2),		// defined by the game dll
	cvar_client_dll					= (1 << 3),		// defined by the client dll
	cvar_hidden						= (1 << 4),		// hidden. doesn't appear in find or autocomplete. like developmentonly, but can't be compiled out.

	// convar only
	cvar_protected					= (1 << 5),		// it's a server cvar, but we don't send the data since it's a password, etc.  sends 1 if it's not bland/zero, 0 otherwise as value
	cvar_sponly						= (1 << 6),		// this cvar cannot be changed by clients connected to a multiplayer server.
	cvar_archive					= (1 << 7),		// set to cause it to be saved to vars.rc
	cvar_notify						= (1 << 8),		// notifies players when changed
	cvar_userinfo					= (1 << 9),		// changes the client's info string
	cvar_cheat						= (1 << 14),	// only useable in singleplayer / debug / multiplayer & sv_cheats
	cvar_printable_only				= (1 << 10),	// this cvar's string cannot contain unprintable characters ( e.g., used for player name etc ).
	cvar_unlogged					= (1 << 11),	// if this is a fcvar_server, don't log changes to the log file / console if we are creating a log
	cvar_never_as_string			= (1 << 12),	// never try to print that cvar

	// it's a convar that's shared between the client and the server.
	// at signon, the values of all such convars are sent from the server to the client (skipped for local client, ofc )
	// if a change is requested it must come from the console (i.e., no remote client changes)
	// if a value is changed while a server is active, it's replicated to all connected clients
	cvar_server						= (1 << 13),	// server setting enforced on clients, replicated
	cvar_demo						= (1 << 16),	// record this cvar when starting a demo file
	cvar_dont_record				= (1 << 17),	// don't record these command in demofiles
	cvar_reload_materials			= (1 << 20),	// if this cvar changes, it forces a material reload
	cvar_reload_textures			= (1 << 21),	// if this cvar changes, if forces a texture reload
	cvar_not_connected				= (1 << 22),	// cvar cannot be changed by a client that is connected to a server
	cvar_material_system_thread		= (1 << 23),	// indicates this cvar is read from the material system thread
	cvar_archive_xbox				= (1 << 24),	// cvar written to config.cfg on the xbox
	cvar_accessible_from_threads	= (1 << 25),	// used as a debugging tool necessary to check material system thread convars
	cvar_server_can_execute			= (1 << 28),	// the server is allowed to execute this command on clients via clientcommand/net_stringcmd/cbaseclientstate::processstringcmd.
	cvar_server_cannot_query		= (1 << 29),	// if this is set, then the server is not allowed to query this cvar's value (via iserverpluginhelpers::startquerycvarvalue).
	cvar_clientcmd_can_execute		= (1 << 30),	// ivengineclient::clientcmd is allowed to execute this command. 
	cvar_material_thread_mask		= (cvar_reload_materials | cvar_reload_textures | cvar_material_system_thread)
};

#pragma endregion

typedef int game_convar_dll_identifier;
class convar_cmd;
class convar_base;

class game_engine_convar : public game_app_system
{
public:
	virtual game_convar_dll_identifier	allocate_dll_identifier() = 0;
	virtual void						register_command(game_convar* command_base, int default_value = 1) = 0;
	virtual void						unregister_command(game_convar* command_base) = 0;
	virtual void						unregister_commands(game_convar_dll_identifier id) = 0;
	virtual const char*					get_command_value(const char* variable_name) = 0;
	virtual convar_base*				find_command_base(const char* name) = 0;
	virtual const convar_base*			find_command_base(const char* name) const = 0;
	virtual game_convar*				find(const char* variable_name) = 0;
	virtual const game_convar*			find(const char* variable_name) const = 0;
	virtual convar_cmd*					find_command(const char* name) = 0;
	virtual const convar_cmd*			find_command(const char* name) const = 0;
	virtual void						install_global_change_callback(change_callback callback) = 0;
	virtual void						remove_global_change_callback(change_callback callback) = 0;
	virtual void						call_global_change_callbacks(game_convar* var, const char* old_string, float old_value) = 0;
	virtual void						install_console_display_func(void* display_func) = 0;
	virtual void						remove_console_display_func(void* display_func) = 0;
	virtual void						console_color_printf(const color& color, const char* format, ...) const = 0;
	virtual void						console_printf(const char* format, ...) const = 0;
	virtual void						console_dprintf(const char* format, ...) const = 0;
	virtual void						revert_flagged_convars(int flag) = 0;
};