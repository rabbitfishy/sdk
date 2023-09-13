#include "../dependency.h"

game_modules* modules = new game_modules;

void game_modules::setup()
{
	client				= ("client.dll");
	engine				= ("engine.dll");
	vgui2				= ("vgui2.dll");
	vgui_surface		= ("vguimatsurface.dll");
	material_system		= ("materialsystem.dll");
	directx9			= ("shaderapidx9.dll");
	data_cache			= ("datacache.dll");
	matchmaking			= ("matchmaking.dll");
	physics				= ("vphysics.dll");
	studio_render		= ("studiorender.dll");
	tier0				= ("tier0.dll");
	localize			= ("localize.dll");
	stdlib				= ("vstdlib.dll");
	input_system		= ("inputsystem.dll");
	server				= ("server.dll");
	server_browser		= ("serverbrowser.dll");

	console->log(log_type::log_success, "modules intsalled!");
}

game_scanner* scanner = new game_scanner;

MODULEENTRY32 game_scanner::capture_module(const char* modules)
{
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, GetCurrentProcessId());

	if (snapshot != INVALID_HANDLE_VALUE) {
		MODULEENTRY32 entry;
		entry.dwSize = sizeof(entry);
		if (Module32First(snapshot, &entry)) {
			do {
				if (!std::strcmp(entry.szModule, modules)) {
					CloseHandle(snapshot);
					return entry;
				}
			} while (Module32Next(snapshot, &entry));
		}
	}

	MODULEENTRY32 module = { DWORD(-1) };
	return module;
}

DWORD game_scanner::scan(const char* modules, const char* signature)
{
	MODULEENTRY32 modules_entry = this->capture_module(modules);

	if (!modules_entry.hModule) 
		return FALSE;

	DWORD start = (DWORD)modules_entry.modBaseAddr;
	DWORD end = start + modules_entry.modBaseSize;
	DWORD first_match = 0;
	const char* p = signature;

	for (DWORD i = start; i < end; i++)
	{
		if (!*p)
			return first_match;

		if (*(PBYTE)p == '\?' || *(BYTE*)i == data_byte(p))
		{
			if (!first_match)
				first_match = i;

			if (!p[2])
				return first_match;

			if (*(PWORD)p == '\?\?' || *(PBYTE)p != '\?')
				p += 3;
			else
				p += 2;
		}
		else
		{
			p = signature;
			first_match = 0;
		}
	}

	return FALSE;
}

game_virtuals* virtuals = new game_virtuals;
