#pragma once

class game_modules
{
public:
	void setup();

	const char* client				= nullptr;
	const char* engine				= nullptr;
	const char* vgui2				= nullptr;
	const char* vgui_surface		= nullptr;
	const char* material_system		= nullptr;
	const char* directx9			= nullptr;
	const char* data_cache			= nullptr;
	const char* matchmaking			= nullptr;
	const char* physics				= nullptr;
	const char* studio_render		= nullptr;
	const char* tier0				= nullptr;
	const char* localize			= nullptr;
	const char* stdlib				= nullptr;
	const char* input_system		= nullptr;
	const char* server				= nullptr;
	const char* server_browser		= nullptr;
};

extern game_modules* modules;