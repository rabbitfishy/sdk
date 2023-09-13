#pragma once
#include "../dependencies/console/console.h"
#include "../valve/interface/interface.h"
#include "../dependencies/dependency.h"
#include <minhook.h>

// add hooks indexes here.
enum hk_index : int
{
	alloc_key_value_memory = 2,
	create_move = 22,
	paint_traverse = 41,
};

/*
* add hooks linker here.
* example: typedef type_parameter(call_parameter* name_linker)(parameters);
* NOTE: the 'call_parameter' is your hook functions call parameter(such as __stdcall, __fastcall and etc...) and
* the 'type_parameter' is your hook functions type parameter(such as void, bool, int and etc...),
* so you replace it with whatever your hook parameter is.
*/
struct hk_linker
{
	typedef void* (__thiscall* lnk_alloc_key_value_memory)(game_key_value_system*, int);
	typedef void(__thiscall* lnk_create_move_proxy)(game_base_client*, int, float, bool);
	typedef void(__thiscall* lnk_paint_traverse)(game_panel*, v_panel, bool, bool);
};

// add your standard hooking functions here.
struct hk_functions
{
	static void* __stdcall alloc_key_value_memory(int size);
	static void __stdcall create_move_proxy(int sequence, float input_sample_frametime, bool active);
	static void __stdcall paint_traverse(v_panel panel, bool force_repaint, bool allow_force);
};

/*
* add your hooked functions original variable here.
* example: hk_linker::name_linker name_original = nullptr;
*/
struct hk_originals : hk_linker
{
	lnk_alloc_key_value_memory alloc_key_value_memory_original	= nullptr;
	lnk_create_move_proxy create_move_original					= nullptr;
	lnk_paint_traverse paint_traverse_original					= nullptr;
};

extern hk_originals* originals;

class game_hooks
{
public:
	void setup();
	void restore();

private:
	/*
	* create hook wrapper.
	* it allows you to remove annoying reinterpret_casts.
	* credit: https://www.codeproject.com/Articles/44326/MinHook-The-Minimalistic-x-x-API-Hooking-Libra
	*/
	template <typename t>
	MH_STATUS create_hook(std::string hook_name, LPVOID target_hook, LPVOID function_hook, t** original_hook)
	{
		/*
		* void ptr to string conversion help credit: https://stackoverflow.com/a/3077027
		* if needed to convert void* to string use. ^^^
		*/

		MH_STATUS create = MH_CreateHook(target_hook, function_hook, (LPVOID*)original_hook);

		if (create != MH_STATUS::MH_OK)
			console->log(log_type::log_error, "failed to hook %s!", hook_name.c_str());

		return create;
	}
};

extern game_hooks* hooks;