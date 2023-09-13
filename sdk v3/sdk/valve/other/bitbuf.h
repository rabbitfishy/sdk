#pragma once
#include <Windows.h>

class bf_write
{
public:
	bf_write() = default;

	// the current buffer.
	unsigned long* data;
	int				data_bytes;
	int				data_bits;

	// where we are in the buffer.
	int				current_bit;
};

class bf_read
{
public:
	std::uintptr_t base_address;
	std::uintptr_t current_offset;

	bf_read(std::uintptr_t address) : base_address(address), current_offset(0U) { }

	void set_offset(std::uintptr_t offset) { current_offset = offset; }
	void skip(std::uintptr_t length) { current_offset += length; }

	int read_byte()
	{
		char value = *reinterpret_cast<char*>(base_address + current_offset);
		current_offset++;
		return value;
	}

	bool read_bool()
	{
		bool value = *reinterpret_cast<bool*>(base_address + current_offset);
		current_offset++;
		return value;
	}

	const char* read_string()
	{
		char buffer[256];
		char length = *reinterpret_cast<char*>(base_address + current_offset);
		current_offset++;
		std::memcpy(buffer, reinterpret_cast<void*>(base_address + current_offset), length > 255 ? 255 : length);
		buffer[length > 255 ? 255 : length] = '\0';
		current_offset += length + 1;
		return buffer;
	}
};