#pragma once
#include "../../netvar/netvar.h"

typedef DWORD h_font;

#pragma region surface flags

enum font_draw_type : int
{
	font_draw_default		= 0,
	font_draw_non_additive,
	font_draw_additive,
	font_draw_type_count	= 2
};

enum font_flags
{
	font_none,
	font_italic			= 0x001,
	font_underline		= 0x002,
	font_strikeout		= 0x004,
	font_symbol			= 0x008,
	font_antialias		= 0x010,
	font_gaussian_blur	= 0x020,
	font_rotary			= 0x040,
	font_dropshadow		= 0x080,
	font_additive		= 0x100,
	font_outline		= 0x200,
	font_custom			= 0x400,
	font_bitmap			= 0x800
};

#pragma endregion

class game_surface
{
public:
	void	draw_set_color(int r, int g, int b, int a) { virtuals->call<void>(this, 15, r, g, b, a); }
	void	draw_filled_rect(int x, int y, int w, int h) { virtuals->call<void>(this, 16, x, y, w, h); }
	void	draw_filled_rect_fade(int x, int y, int w, int h, std::uint32_t first, std::uint32_t second, bool horizontal) { virtuals->call<void>(this, 123, x, y, w, h, first, second, horizontal); }
	void	draw_outlined_rect(int x, int y, int w, int h) { virtuals->call<void>(this, 18, x, y, w, h); }
	void	draw_line(int x, int y, int w, int h) { virtuals->call<void>(this, 19, x, y, w, h); }
	void	draw_polyline(int* x, int* y, int points) { virtuals->call<void>(this, 20, x, y, points); }
	void	draw_set_text_font(h_font font_handle) { virtuals->call<void>(this, 23, font_handle); }
	void	draw_set_text_color(int r, int g, int b, int a) { virtuals->call<void>(this, 25, r, g, b, a); }
	void	draw_set_text_pos(int x, int y) { virtuals->call<void>(this, 26, x, y); }
	void	draw_print_text(const wchar_t* text, int text_length, font_draw_type draw_type = font_draw_default) { virtuals->call<void>(this, 28, text, text_length, draw_type); }
	void	draw_set_texture_rgba(int index, const unsigned char* rgba, int width, int height) { virtuals->call<void>(this, 37, index, rgba, width, height); }
	void	draw_set_texture(int index) { virtuals->call<void>(this, 38, index); }
	int		create_new_texture_id(bool procedural = false) { return virtuals->call<int>(this, 43, procedural); }
	void	unlock_cursor() { return virtuals->call<void>(this, 66); }
	void	lock_cursor() { return virtuals->call<void>(this, 67); }
	h_font	font_create() { return virtuals->call<h_font>(this, 71); }
	bool	set_font_glyph_set(h_font font_handle, const char* windows_font_name, int height, int weight, int blur, int scan_lines, int flags, int range_min = 0, int range_max = 0) { return virtuals->call<bool>(this, 72, font_handle, windows_font_name, height, weight, blur, scan_lines, flags, range_min, range_max); }
	void	get_text_size(h_font font_handle, const wchar_t* text, int& width, int& height) { virtuals->call<void>(this, 79, font_handle, text, std::ref(width), std::ref(height)); }
	void	playsound_surface(const char* file_name) { virtuals->call<void>(this, 82, file_name); }
	void	draw_outlined_circle(int x, int y, int radius, int segments) { virtuals->call<void>(this, 103, x, y, radius, segments); }
	void	draw_textured_polygon(int index, vertex* vertice, bool clip_vertices = true) { virtuals->call<void>(this, 106, index, vertice, clip_vertices); }

	// ref: https://www.unknowncheats.me/forum/2547880-post10.html
	// ref: https://github.com/otvv/csgo-linux-cheat-sdk/blob/cc3bc2d6ae4ab3e305f681228728083db6d6de7f/csgo-sdk/src/interfaces/isurface.hpp#L191
	void	set_clip_rect(int x, int y, int w, int h) { return virtuals->call<void>(this, 147, x, y, w, h); }
	void	disable_clip(bool disable) { this->enable_clip() = !disable; }

private:
	// ref: https://www.unknowncheats.me/forum/2450313-post1.html
	// ( 0x280 = 0xA0 * 4 )
	OFFSET(enable_clip, bool, 0x280)
};
