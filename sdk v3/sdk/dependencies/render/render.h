#pragma once
#include "../console/console.h"
#include "../../valve/interface/interface.h"
#include "../../valve/maths/other.h"
#include "../other/translate.h"

enum position_flags : int
{
	x_right		= 1,
	x_centre	= 1 << 1,

	y_bottom	= 1 << 2,
	y_centre	= 1 << 3
};

enum gradient_direction : bool
{
	vertical	= false,
	horizontal	= true
};

struct game_fonts
{
	h_font tahoma;
	h_font verdana;
	h_font small_font;
};

extern game_fonts* fonts;

class game_render
{
public:
	void setup();

	void line(int x, int y, int w, int h, color color);
	void filled_rect(int x, int y, int w, int h, color color, position_flags flags = { });
	void outlined_rect(int x, int y, int w, int h, color color, position_flags flags = { });
	void gradient(int x, int y, int w, int h, color first, color second, gradient_direction direction = { });
	void text(h_font& font, int x, int y, std::string text, color color, position_flags flags = { });
	void wtext(h_font& font, int x, int y, std::wstring text, color color, position_flags flags = { });

	void start_clip(rect area);
	void end_clip();

	dimension text_size(h_font font, std::string text);
	dimension wtext_size(h_font font, std::wstring text);
	dimension screen = { };

private:
	void create_font(h_font& font, std::string name, int height, int weight, int flags = 0);
	void handle_flags(int& x, int& y, int width, int height, int flags);
	void setup_screen();
};

extern game_render* render;