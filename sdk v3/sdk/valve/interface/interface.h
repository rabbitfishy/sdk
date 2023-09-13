#pragma once
#include <Windows.h>
#include <string>
#include "../../dependencies/memory/scanner.h"
#include "../../dependencies/memory/virtual.h"
#include "../../dependencies/other/color.h"
#include "../maths/q_angle.h"
#include "../maths/utl_vector.h"
#include "../maths/vector.h"
#include "../maths/matrix.h"
#include "../maths/other.h"
#include "../other/net_channel.h"
#include "../other/studio.h"
#include "../other/convar.h"
#include "../other/weapon_info.h"
#include "../other/key_value.h"
#include "../entity/other.h"

#pragma region client.dll

#include "classes/base_client.h"
#include "classes/entity_list.h"
#include "classes/game_console.h"

#pragma endregion

#include "classes/panel.h"

#pragma region engine.dll

#include "classes/engine_client.h"
#include "classes/engine_trace.h"
#include "classes/engine_sound.h"
#include "classes/engine_vgui.h"
#include "classes/network.h"
#include "classes/event_manager.h"
#include "classes/debug_overlay.h"
#include "classes/model_info.h"

#pragma endregion

#pragma region vgui.dll

#include "classes/surface.h"

#pragma endregion

#include "classes/client_mode.h"

#pragma region other

#include "classes/engine_convar.h"
#include "classes/localize.h"
#include "classes/game_types.h"
#include "classes/material_system.h"
#include "classes/studio_render.h"
#include "classes/physics_surface_props.h"

#pragma endregion

#pragma region custom

#include "classes/global_vars.h"
#include "classes/key_value_system.h"
#include "classes/weapon_system.h"
#include "classes/input.h"

#pragma endregion

#include "classes/prediction.h"
#include "classes/base_interface.h"
#include "../../dependencies/console/console.h"

class game_interface
{
public:
	void setup();

#pragma region client.dll

	game_base_client* base_client				= nullptr;
	game_entity_list* entity_list				= nullptr;
	game_client_console* game_console			= nullptr;
	game_prediction* prediction					= nullptr;
	game_movement* movement						= nullptr;

#pragma endregion

#pragma region engine.dll

	game_engine_client* engine_client			= nullptr;
	game_engine_trace* engine_trace				= nullptr;
	game_engine_sound* engine_sound				= nullptr;
	game_engine_vgui* engine_vgui				= nullptr;
	game_network_container* network_container	= nullptr;
	game_event_manager* game_event				= nullptr;
	game_debug_overlay* debug_overlay			= nullptr;
	game_model_info* model_info					= nullptr;

#pragma endregion

#pragma region vgui.dll

	game_panel* panel							= nullptr;
	game_surface* surface						= nullptr;

#pragma endregion

#pragma region others

	game_engine_convar* convar					= nullptr;
	game_localize* localize						= nullptr;
	game_client_types* game_types				= nullptr;
	game_material_system* material_system		= nullptr;
	game_studio_render* studio_render			= nullptr;
	game_input_system* input_system				= nullptr;
	game_physics_surface_props* physics			= nullptr;

#pragma endregion

#pragma region custom

	game_client_mode* client_mode				= nullptr;
	game_global_vars* global_vars				= nullptr;
	game_key_value_system* key_value_system		= nullptr;
	game_client_state* client_state				= nullptr;
	game_weapon_system* weapon_system			= nullptr;
	game_input* input							= nullptr;

#pragma endregion

private:
	static interface_register* register_list(const char* modules)
	{
		std::uint8_t* create_interface = nullptr;

		if (const HMODULE handle = scanner->capture_module(modules).hModule; handle != nullptr)
			create_interface = reinterpret_cast<std::uint8_t*>(GetProcAddress(handle, "CreateInterface"));

		if (create_interface == nullptr)
		{
			console->log(log_type::log_error, "failed to get 0x%02X", create_interface);
			return nullptr;
		}

		return ADDRESS(create_interface).rel32(0x5).add(0x6).deref<interface_register*>(dereference::twice);
	}

	template<typename t>
	t* create(const interface_register* register_module, const std::string_view version)
	{
		for (const interface_register* register_list = register_module; register_list != nullptr; register_list = register_list->next)
		{
			if ((!std::string(register_list->name).compare(0U, version.length(), version) && std::atoi(register_list->name + version.length()) > 0) || !version.compare(register_list->name))
			{
				// capture our interface.
				void* capture = register_list->create();
				return static_cast<t*>(capture);
			}
		}

		console->log(log_type::log_error, "failed to find %s", version);
		return nullptr;
	}
};

extern game_interface* interfaces;