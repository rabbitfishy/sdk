#include "../hooks.h"

void __stdcall hk_functions::paint_traverse(v_panel panel, bool force_repaint, bool allow_force)
{
	hash32 draw_panel = HASH_COMPILE(interfaces->panel->name(panel));

	switch (draw_panel)
	{
	case HASH_COMPILE("MatSystemTopPanel"):
		// drawing functions here.

		
		
		break;
	}

	originals->paint_traverse_original(interfaces->panel, panel, force_repaint, allow_force);
}
