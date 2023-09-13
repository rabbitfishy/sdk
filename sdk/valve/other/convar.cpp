#include "convar.h"
#include "../valve.h"

game_spoofed_convar::game_spoofed_convar(const char* var)
{
	original_convar = interfaces->convar->find(var);
	spoof();
}

game_spoofed_convar::game_spoofed_convar(game_convar* var)
{
	original_convar = var;
	spoof();
}

game_spoofed_convar::~game_spoofed_convar()
{
	if (is_spoofed())
	{
		// restore values.
		set_flags(original_flags);
		set(original_value); // string values.

		// restore original name.
		game_protect protect = PROTECT((LPVOID)original_convar->name, 128UL, PAGE_READWRITE);
		strcpy_s(const_cast<char*>(original_convar->name), sizeof(original_name), original_name);

		// unregister temporary convar.
		interfaces->convar->unregister_command(dummy_convar);
		free(dummy_convar);
		dummy_convar = nullptr;
	}
}

bool game_spoofed_convar::is_spoofed() const
{
	return dummy_convar != nullptr;
}

void game_spoofed_convar::spoof()
{
	if (!is_spoofed() && original_convar != nullptr)
	{
		// backup old values.
		original_flags = original_convar->flags;
		strcpy_s(original_name, original_convar->name);
		strcpy_s(original_value, original_convar->default_value);
		sprintf_s(dummy_name, 128U, "%s", original_name);

		// create temporary convar.
		dummy_convar = static_cast<game_convar*>(malloc(sizeof(game_convar)));

		if (dummy_convar == nullptr)
			return;

		memcpy(dummy_convar, original_convar, sizeof(game_convar));
		dummy_convar->next = nullptr;

		// register temporary convar.
		interfaces->convar->register_command(dummy_convar);

		game_protect protect = PROTECT((LPVOID)original_convar->name, 128UL, PAGE_READWRITE);

		// rename the convar.
		strcpy_s(const_cast<char*>(original_convar->name), sizeof(dummy_name), dummy_name);

		set_flags(cvar_none);
	}
}

void game_spoofed_convar::set_flags(int flags) const
{
	if (is_spoofed())
		original_convar->flags = flags;
}

int game_spoofed_convar::get_flags() const
{
	return original_convar->flags;
}

void game_spoofed_convar::set(bool value) const
{
	if (is_spoofed())
		original_convar->set(value);
}

void game_spoofed_convar::set(int value) const
{
	if (is_spoofed())
		original_convar->set(value);
}

void game_spoofed_convar::set(float value) const
{
	if (is_spoofed())
		original_convar->set(value);
}

void game_spoofed_convar::set(const char* value) const
{
	if (is_spoofed())
		original_convar->set(value);
}