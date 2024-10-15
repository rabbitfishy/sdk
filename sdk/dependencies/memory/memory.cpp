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

	// remove all the spaces in the signature array.
	std::string sig = translate->remove_space(signature);

	// get our start and end of memory.
	DWORD start = (DWORD)modules_entry.modBaseAddr;
	DWORD end	= start + modules_entry.modBaseSize;

	// keep count of our pattern matches.
	int first_match = 0;

	// store our pattern for comparing later.
	const char* pattern = sig.c_str();

	for (DWORD i = start; i < end; i++)
	{
		// our pattern don't match in game memory section then return.
		if (!pattern)
			return first_match;

		// our pattern found a wild card or the pattern itself matched.
		if (*pattern == '\?' || i == data_byte(pattern))
		{
			// we haven't found our first match yet then loop through the pattern array.
			if (!first_match)
				first_match = i;

			// isn't our second pattern index then return the first match.
			if (!pattern[2])
				return first_match;

			// our pattern have 2 wild cards then extend 3 indexes ahead.
			// otherwise we don't have a wild card then extend it by 2 indexes ahead.
			if (*pattern == '\?\?' || *pattern != '\?')
				pattern += 3;
			else
				pattern += 2;
		}
		else
		{
			// all those conditions haven't been met then start all over again.
			pattern		= sig.c_str();
			first_match = 0;
		}
	}

	return FALSE;
}

game_virtuals* virtuals = new game_virtuals;
