#pragma once
#include "../console/console.h"
#include "../../valve/interface/interface.h"
#include "../../valve/maths/other.h"
#include "../other/translate.h"
#include "font.h"

enum gradient_orientation : int
{
	vertical	= 0,
	horizontal	= 1
};

struct game_fonts
{
	render_font tahoma;
	render_font verdana;
	render_font small_font;
};

extern game_fonts* fonts;

class game_render
{
public:
	void setup();

	void line(int x, int y, int w, int h, color color);
	void filled_rect(int x, int y, int w, int h, color color);
	void outlined_rect(int x, int y, int w, int h, color color);
	void gradient(int x, int y, int w, int h, color first, color second, gradient_orientation orientation = vertical);

	void start_clip(rect area);
	void end_clip();

	dimension screen = { };

private:
	void setup_screen();
};

extern game_render* render;
