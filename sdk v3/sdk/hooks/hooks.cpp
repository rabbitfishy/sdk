#include "hooks.h"

game_hooks* hooks = new game_hooks;
hk_originals* originals = new hk_originals;

void game_hooks::setup()
{
	// install hooking library.
	if (MH_Initialize() != MH_STATUS::MH_OK)
		console->log(log_type::log_error, "failed to install minhook!");

	// virtual hook target.
	void* alloc_key_value_memory_target		= virtuals->get(interfaces->key_value_system, hk_index::alloc_key_value_memory);
	void* create_move_target				= virtuals->get(interfaces->base_client, hk_index::create_move);
	void* paint_traverse_target				= virtuals->get(interfaces->panel, hk_index::paint_traverse);

	// add hooks here.
	create_hook("alloc_key_value_memory", alloc_key_value_memory_target, hk_functions::alloc_key_value_memory, &originals->alloc_key_value_memory_original);
	create_hook("create_move", create_move_target, hk_functions::create_move_proxy, &originals->create_move_original);
	create_hook("paint_traverse", paint_traverse_target, hk_functions::paint_traverse, &originals->paint_traverse_original);

	// then enable hooking.
	MH_EnableHook(MH_ALL_HOOKS);

	console->log(log_type::log_success, "hooks intsalled!");
}

void game_hooks::restore()
{
	// disable hooking library.
	MH_DisableHook(MH_ALL_HOOKS);

	// then remove hook(s).
	MH_RemoveHook(MH_ALL_HOOKS);

	// then uninstall hooking library.
	MH_Uninitialize();
}