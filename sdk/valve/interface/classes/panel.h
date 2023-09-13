#pragma once

typedef DWORD v_panel;

class game_panel
{
public:
	void			set_keyboard_input(v_panel panel, bool state) { return virtuals->call<void>(this, 31, panel, state); }
	void			set_mouse_input(v_panel panel, bool state) { return virtuals->call<void>(this, 32, panel, state); }
	const char*		name(v_panel panel) { return virtuals->call<const char*>(this, 36, panel); }
	const char*		name_class(v_panel panel) { return virtuals->call<const char*>(this, 37, panel); }
};