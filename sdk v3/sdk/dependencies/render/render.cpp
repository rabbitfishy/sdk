#include "render.h"

game_render* render = new game_render;
game_fonts* fonts = new game_fonts;

void game_render::setup()
{
	// create font here.
	this->create_font(fonts->tahoma, "tahoma", 12, FW_NORMAL, fontflag_dropshadow);
	this->create_font(fonts->verdana, "verdana", 12, FW_NORMAL, fontflag_antialias | fontflag_dropshadow);
	this->create_font(fonts->small_font, "small fonts", 8, FW_NORMAL, fontflag_outline);

	// calculate screen size.
	this->setup_screen();

	console->log(log_type::log_success, "render intsalled!");
}

void game_render::line(int x, int y, int w, int h, color color)
{
	interfaces->surface->draw_set_color(color.r, color.g, color.b, color.a);
	interfaces->surface->draw_line(x, y, w, h);
}

void game_render::filled_rect(int x, int y, int w, int h, color color, position_flags flags)
{
	interfaces->surface->draw_set_color(color.r, color.g, color.b, color.a);
	this->handle_flags(x, y, w, h, flags);
	interfaces->surface->draw_filled_rect(x, y, x + w, y + h);
}

void game_render::outlined_rect(int x, int y, int w, int h, color color, position_flags flags)
{
	interfaces->surface->draw_set_color(color.r, color.g, color.b, color.a);
	this->handle_flags(x, y, w, h, flags);
	interfaces->surface->draw_outlined_rect(x, y, x + w, y + h);
}

void game_render::gradient(int x, int y, int w, int h, color first, color second, gradient_direction direction)
{
	interfaces->surface->draw_set_color(first.r, first.g, first.b, first.a);
	interfaces->surface->draw_filled_rect_fade(x, y, x + w, y + h, 255, 255, direction);

	interfaces->surface->draw_set_color(second.r, second.g, second.b, second.a);
	interfaces->surface->draw_filled_rect_fade(x, y, x + w, y + h, 0, 255, direction);
}

void game_render::text(h_font& font, int x, int y, std::string text, color color, position_flags flags)
{
	if (flags)
	{
		static dimension text_size = { };
		interfaces->surface->get_text_size(font, translate->unicode(text).c_str(), text_size.w, text_size.h);
		this->handle_flags(x, y, text_size.w, text_size.h, flags);
	}

	interfaces->surface->draw_set_text_color(color.r, color.g, color.b, color.a);
	interfaces->surface->draw_set_text_font(font);
	interfaces->surface->draw_set_text_pos(x, y);
	interfaces->surface->draw_print_text(translate->unicode(text).c_str(), text.length());
}

void game_render::wtext(h_font& font, int x, int y, std::wstring text, color color, position_flags flags)
{
	if (flags)
	{
		static dimension text_size = { };
		interfaces->surface->get_text_size(font, text.c_str(), text_size.w, text_size.h);
		this->handle_flags(x, y, text_size.w, text_size.h, flags);
	}

	interfaces->surface->draw_set_text_color(color.r, color.g, color.b, color.a);
	interfaces->surface->draw_set_text_font(font);
	interfaces->surface->draw_set_text_pos(x, y);
	interfaces->surface->draw_print_text(text.c_str(), text.length());
}

dimension game_render::text_size(h_font font, std::string text)
{
	static dimension text_area = { };
	interfaces->surface->get_text_size(font, translate->unicode(text).c_str(), text_area.w, text_area.h);
	return text_area;
}

dimension game_render::wtext_size(h_font font, std::wstring text)
{
	static dimension text_area = { };
	interfaces->surface->get_text_size(font, text.c_str(), text_area.w, text_area.h);
	return text_area;
}

void game_render::start_clip(rect area)
{
	rect canvas = { area.x, area.y, area.x + area.w, area.y + area.h };
	interfaces->surface->disable_clip(false);
	interfaces->surface->set_clip_rect(canvas.x, canvas.y, canvas.w, canvas.h);
}

void game_render::end_clip()
{
	interfaces->surface->disable_clip(true);
}

void game_render::create_font(h_font& font, std::string name, int height, int weight, int flags)
{
	font = interfaces->surface->font_create();
	interfaces->surface->set_font_glyph_set(font, name.c_str(), height, weight, 0, 0, flags);
}

void game_render::handle_flags(int& x, int& y, int width, int height, int flags)
{
	if (!flags) return;

	switch (flags)
	{
	case x_right:  x -= width;		break;
	case x_centre: x -= width / 2;	break;
	case y_bottom: y -= height;		break;
	case y_centre: y -= height / 2; break;
	}
}

void game_render::setup_screen()
{
	dimension canvas = { 0, 0 };
	interfaces->engine_client->get_screen_size(canvas.w, canvas.h);
	this->screen = { canvas.w, canvas.h };
}