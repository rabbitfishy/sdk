#pragma once

// all interfaces derive from this.
class game_base_interface
{
public:
	virtual ~game_base_interface() { }
};

using instantiate_interface = void* (__cdecl*)();

// used internally to register interface classes.
class interface_register
{
public:
	instantiate_interface	create; // 0x00
	const char*				name;	// 0x04
	interface_register*		next;	// 0x08
};