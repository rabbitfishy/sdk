#pragma once
#include <Windows.h>
#include <string>
#include <algorithm>
#include <sstream>
#include <bitset>

class text_translate
{
public:
	// used for weapon esp name cleaning.
	std::wstring clean(std::wstring input)
	{
		std::wstring clean = this->add_space(this->replace(input.data()));
		return clean;
	}

	// converting string to uppercase - so we don't have to write it out manually everytime we need to convert to uppercase characters.
	std::string upper(std::string input)
	{
		std::transform(input.begin(), input.end(), input.begin(), ::toupper);
		return input;
	}

	std::wstring upper(std::wstring input)
	{
		std::transform(input.begin(), input.end(), input.begin(), ::toupper);
		return input;
	}

	// convert unicode to ascii (utf8).
	std::string ascii(std::wstring input)
	{
		if (input.empty()) 
			return std::string();

		const auto size = WideCharToMultiByte(CP_UTF8, 0, input.data(), input.size(), 0, 0, 0, 0);
		auto utf8 = std::string(size, 0);
		WideCharToMultiByte(CP_UTF8, 0, input.data(), input.size(), utf8.data(), size, 0, 0);

		return utf8;
	}

	// convert ascii to unicode (utf16).
	std::wstring unicode(std::string input)
	{
		if (input.empty()) 
			return std::wstring();

		const auto size = MultiByteToWideChar(CP_UTF8, 0, input.data(), input.size(), 0, 0);
		auto unicode = std::wstring(size, 0);
		MultiByteToWideChar(CP_UTF8, 0, input.data(), input.size(), unicode.data(), size);

		return unicode;
	}

	/*
	* converting compressed fonts in c array or converting any binary data in a c array,
	* results may vary depends on what is being converted. mainly used to convert fonts.
	* NOTE: this algorithm doesn't account for glyphs thats in the font.
	* credit: https://stackoverflow.com/a/23344876
	*/
	std::string binary(unsigned char* binary_data)
	{
		std::string ascii_data;
		std::stringstream ss(*binary_data);

		while (ss.good()) 
		{
			std::bitset<8> bits;			// get 8 bit data set. (since 8 bit equals to one character)
			ss >> bits;				// divide binary data by bits two times.
			ascii_data += char(bits.to_ulong());	// convert those bits and assign it to the string variable to store converted data.
		}

		return ascii_data;
	}

	// format decimal number to string.
	std::string format(const char* layout, ...)
	{
		char buffer[256];
		if (std::strlen(layout) >= sizeof(buffer))
			return std::string();

		va_list arguments;
		va_start(arguments, layout);
		vsprintf_s(buffer, sizeof(buffer), layout, arguments);
		std::string result = buffer;
		va_end(arguments);

		return result;
	}

private:
	// find space then add space to it - if it makes sense lmao.
	std::wstring add_space(const std::string& input)
	{
		std::wstring wide_string;

		for (std::size_t i = 0; i < input.length(); i++) 
		{
			if (isspace(input[i]) && i != 0)
				wide_string += this->unicode("  ");
			wide_string += input[i];
		}

		return wide_string;
	}

	// replace '-' with space.
	std::string replace(std::wstring input)
	{
		std::replace(input.begin(), input.end(), '-', ' ');
		std::string convert = std::string(input.begin(), input.end());
		return convert;
	}
};

extern text_translate* translate;
