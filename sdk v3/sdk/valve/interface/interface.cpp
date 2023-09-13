#include "interface.h"
#include "../valve.h"

game_interface* interfaces = new game_interface;

void game_interface::setup()
{
#pragma region client.dll

	const interface_register* client_register = this->register_list(modules->client);
	base_client			= create<game_base_client>(client_register, "VClient");
	entity_list			= create<game_entity_list>(client_register, "VClientEntityList");
	game_console		= create<game_client_console>(client_register, "GameConsole");
	prediction			= create<game_prediction>(client_register, "VClientPrediction");
	movement			= create<game_movement>(client_register, "GameMovement");

#pragma endregion

#pragma region engine.dll

	const interface_register* engine_register = this->register_list(modules->engine);
	engine_client		= create<game_engine_client>(engine_register, "VEngineClient");
	engine_trace		= create<game_engine_trace>(engine_register, "EngineTraceClient");
	engine_sound		= create<game_engine_sound>(engine_register, "IEngineSoundClient");
	engine_vgui			= create<game_engine_vgui>(engine_register, "VEngineVGui");
	network_container	= create<game_network_container>(engine_register, "VEngineClientStringTable");
	game_event			= create<game_event_manager>(engine_register, "GAMEEVENTSMANAGER002");
	debug_overlay		= create<game_debug_overlay>(engine_register, "VDebugOverlay");
	model_info			= create<game_model_info>(engine_register, "VModelInfoClient");

#pragma endregion

#pragma region vgui.dll

	const interface_register* vgui2_register = this->register_list(modules->vgui2);
	panel				= create<game_panel>(vgui2_register, "VGUI_Panel");

	const interface_register* vgui_surface_register = this->register_list(modules->vgui_surface);
	surface				= create<game_surface>(vgui_surface_register, "VGUI_Surface");

#pragma endregion

#pragma region others

	const interface_register* stdlib_register = this->register_list(modules->stdlib);
	convar				= create<game_engine_convar>(stdlib_register, "VEngineCvar");

	const interface_register* localize_register = this->register_list(modules->localize);
	localize			= create<game_localize>(localize_register, "Localize_");

	const interface_register* matchmaking_register = this->register_list(modules->matchmaking);
	game_types			= create<game_client_types>(matchmaking_register, "VENGINE_GAMETYPES_VERSION");

	const interface_register* material_system_register = this->register_list(modules->material_system);
	material_system		= create<game_material_system>(material_system_register, "VMaterialSystem");

	const interface_register* studio_render_register = this->register_list(modules->studio_render);
	studio_render		= create<game_studio_render>(studio_render_register, "VStudioRender");

	const interface_register* input_system_register = this->register_list(modules->input_system);
	input_system		= create<game_input_system>(input_system_register, "InputSystemVersion");

	const interface_register* physics_register = this->register_list(modules->physics);
	physics				= create<game_physics_surface_props>(physics_register, "VPhysicsSurfaceProps");

#pragma endregion

#pragma region custom

	client_mode			= **reinterpret_cast<game_client_mode***>(virtuals->get<std::uint8_t*>(base_client, 10) + 0x5);
	global_vars			= **reinterpret_cast<game_global_vars***>(virtuals->get<std::uint8_t*>(base_client, 11) + 0xA);
	key_value_system	= reinterpret_cast<key_value_system_detail>(GetProcAddress(scanner->capture_module(modules->stdlib).hModule, "KeyValuesSystem"))();
	client_state		= SEARCH(modules->engine, signatures::interfaces::client_state::signature()).add(0x1).deref<game_client_state*>(dereference::twice);
	weapon_system		= SEARCH(modules->client, signatures::interfaces::weapon_system::signature()).add(0x2).deref<game_weapon_system*>();
	input				= SEARCH(modules->client, signatures::interfaces::input::signature()).add(0x1).deref<game_input*>();

#pragma endregion

	console->log(log_type::log_success, "interface installed!");
}