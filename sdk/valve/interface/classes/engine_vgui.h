#pragma once

#define invalid_panel 0xFFFFFFFF

enum game_vgui_panel : int
{
	panel_root = 0,
	panel_game_ui_dll,
	panel_client_dll,
	panel_tools,
	panel_in_game_screens,
	panel_game_dll,
	panel_client_dll_tools
};

enum game_paint_mode
{
	paint_ui_panels			= (1 << 0),
	paint_in_game_panels	= (1 << 1),
	paint_cursor			= (1 << 2)
};

class game_engine_vgui
{
public:
	virtual				~game_engine_vgui() { }
	virtual v_panel		panel(game_vgui_panel type) = 0;
	virtual bool		is_game_ui_visible() = 0;
};