#include "render.h"

game_render* render = new game_render;
game_fonts* fonts = new game_fonts;

void game_render::setup()
{
	// create our font here.
	fonts->tahoma		= render_font("tahoma", 12, FW_NORMAL, font_dropshadow);
	fonts->verdana		= render_font("verdana", 12, FW_NORMAL, font_antialias | font_dropshadow);
	fonts->small_font	= render_font("small fonts", 8, FW_NORMAL, font_outline);

	// calculate screen size.
	this->setup_screen();

	console->log(log_type::log_success, "render intsalled!");
}

void game_render::line(int x, int y, int w, int h, color color)
{
	interfaces->surface->draw_set_color(color.r, color.g, color.b, color.a);
	interfaces->surface->draw_line(x, y, w, h);
}

void game_render::filled_rect(int x, int y, int w, int h, color color)
{
	interfaces->surface->draw_set_color(color.r, color.g, color.b, color.a);
	interfaces->surface->draw_filled_rect(x, y, x + w, y + h);
}

void game_render::outlined_rect(int x, int y, int w, int h, color color)
{
	interfaces->surface->draw_set_color(color.r, color.g, color.b, color.a);
	interfaces->surface->draw_outlined_rect(x, y, x + w, y + h);
}

void game_render::gradient(int x, int y, int w, int h, color first, color second, gradient_direction direction)
{
	interfaces->surface->draw_set_color(first.r, first.g, first.b, first.a);
	interfaces->surface->draw_filled_rect_fade(x, y, x + w, y + h, 255, 255, direction);

	interfaces->surface->draw_set_color(second.r, second.g, second.b, second.a);
	interfaces->surface->draw_filled_rect_fade(x, y, x + w, y + h, 0, 255, direction);
}

void game_render::start_clip(rect area)
{
	rect canvas = { area.x, area.y, area.x + area.w, area.y + area.h };
	// turn off clipping as we need to display items.
	interfaces->surface->disable_clip(true);
	interfaces->surface->set_clip_rect(canvas.x, canvas.y, canvas.w, canvas.h);
}

void game_render::end_clip()
{
	// turn on clipping cuz we don't want items to be shown beyond this point.
	interfaces->surface->disable_clip(false);
}

void game_render::setup_screen()
{
	dimension canvas = { 0, 0 };
	interfaces->engine_client->get_screen_size(canvas.w, canvas.h);
	this->screen = { canvas.w, canvas.h };
}
