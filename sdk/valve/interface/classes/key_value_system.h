#pragma once

#define invalid_key_symbol (-1)

typedef unsigned long h_key_symbol;

class game_key_value_system
{
public:
	virtual void			register_size(int reg_size) = 0;

private:
	virtual void			function0() = 0;

public:
	virtual void*			alloc_memory(int mem_size) = 0;
	virtual void			free_memory(void* memory_value) = 0;
	virtual h_key_symbol	symbol_string(const char* name, bool create = true) = 0;
	virtual const char*		string_symbol(h_key_symbol h_symbol) = 0;
	virtual void			add_memory_leak_list(void* memory_value, h_key_symbol h_symbol_name) = 0;
	virtual void			remove_from_memory_leak_list(void* memory_value) = 0;
	virtual void			set_expression_symbol(const char* name, bool value) = 0;
	virtual bool			expression_symbol(const char* name) = 0;
	virtual h_key_symbol	symbol_string_case_sensitive(h_key_symbol& case_insensitive_symbol, const char* name, bool create = true) = 0;
};

using key_value_system_detail = game_key_value_system * (__cdecl*)();