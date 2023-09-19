#pragma once

enum font_alignment : int
{
	align_left		= 1,
	align_right		= 1 << 1,
	align_center	= 1 << 2
};

class render_font
{
public:
	render_font() : handle{ } { }
	render_font(const char* name, int height, int weight, int flags)
	{
		this->handle = interfaces->surface->font_create();
		interfaces->surface->set_font_glyph_set(this->handle, name, height, weight, 0, 0, flags);
	}

	void text(int x, int y, std::string text, color color, font_alignment flags = { })
	{
		if (flags)
		{
			static dimension text_size = { };
			interfaces->surface->get_text_size(this->handle, translate->unicode(text).c_str(), text_size.w, text_size.h);
			this->handle_flags(x, text_size.w, text_size.h, flags);
		}

		interfaces->surface->draw_set_text_color(color.r, color.g, color.b, color.a);
		interfaces->surface->draw_set_text_font(this->handle);
		interfaces->surface->draw_set_text_pos(x, y);
		interfaces->surface->draw_print_text(translate->unicode(text).c_str(), text.length());
	}

	void wtext(int x, int y, std::wstring text, color color, font_alignment flags = { })
	{
		if (flags)
		{
			static dimension text_size = { };
			interfaces->surface->get_text_size(this->handle, text.c_str(), text_size.w, text_size.h);
			this->handle_flags(x, text_size.w, text_size.h, flags);
		}

		interfaces->surface->draw_set_text_color(color.r, color.g, color.b, color.a);
		interfaces->surface->draw_set_text_font(this->handle);
		interfaces->surface->draw_set_text_pos(x, y);
		interfaces->surface->draw_print_text(text.c_str(), text.length());
	}

	dimension text_size(std::string text)
	{
		static dimension text_area = { };
		interfaces->surface->get_text_size(this->handle, translate->unicode(text).c_str(), text_area.w, text_area.h);
		return text_area;
	}

	dimension wtext_size(std::wstring text)
	{
		static dimension text_area = { };
		interfaces->surface->get_text_size(this->handle, text.c_str(), text_area.w, text_area.h);
		return text_area;
	}

private:
	h_font handle;

	void handle_flags(int& x, int width, int height, int flags)
	{
		if (flags & font_alignment::align_right)
			x -= width;

		if (flags & font_alignment::align_center)
			x -= width / 2;
	}
};