#include "key_value.h"
#include "../valve.h"
#include <assert.h>

game_key_value::game_key_value(const char* key_name, void* unknown1, h_key_symbol case_insensitive_key_name)
{
	using param_key_value_constructor = void(__thiscall*)(void*, const char*, void*, h_key_symbol);
	static param_key_value_constructor original_constructor = SEARCH(modules->client, signatures::other::key_value_constructor::signature()).cast<param_key_value_constructor>(); // @xref: client.dll -> "OldParticleSystem_Destroy"
	original_constructor(this, key_name, unknown1, case_insensitive_key_name);
}

game_key_value::~game_key_value()
{
	using param_key_values_destructor = void(__thiscall*)(void*, int);
	static param_key_values_destructor original_destructor = SEARCH(modules->client, signatures::other::key_value_destructor::signature()).cast<param_key_values_destructor>();
	original_destructor(this, 1);
}

void* game_key_value::operator new(std::size_t alloc_size)
{
	// manually allocate memory, because game constructor doesn't call it automatically
	return interfaces->key_value_system->alloc_memory(alloc_size);
}

void game_key_value::operator delete(void* memory_ptr)
{
	// do nothing, because game destructor will automatically free memory
	// I::KeyValuesSystem->FreeKeyValuesMemory(memory_ptr);
	(void)memory_ptr;
}

const char* game_key_value::get_name() { return interfaces->key_value_system->string_symbol(this->key_name_case_sensitive1 | (this->key_name_case_sensitive2 << 8)); }

game_key_value* game_key_value::from_string(const char* name, const char* value)
{
	static auto original_from_string = SEARCH(modules->client, signatures::other::from_string::signature()); // @xref: "#empty#", "#int#"
	game_key_value* key_values = nullptr;

	if (original_from_string == 0U)
		return nullptr;

	__asm
	{
		push		0
		mov edx,	value
		mov ecx,	name
		call		original_from_string
		add esp,	4
		mov			key_values, eax
	}

	return key_values;
}

void game_key_value::load_from_buffer(char const* resource_name, const char* buffer, void* file_system, const char* path_id, symbol_proc evaluate_symbol_proc)
{
	using param_load_from_buffer = void(__thiscall*)(void*, const char*, const char*, void*, const char*, void*, void*);
	static auto original_load_from_buffer = SEARCH(modules->client, signatures::other::load_from_buffer::signature()).cast<param_load_from_buffer>(); // @xref: "KeyValues::LoadFromBuffer(%s%s%s): Begin"
	assert(original_load_from_buffer != nullptr);
	original_load_from_buffer(this, resource_name, buffer, file_system, path_id, evaluate_symbol_proc, nullptr);
}

bool game_key_value::load_from_file(void* file_system, const char* resource_name, const char* path_id, symbol_proc evaluate_symbol_proc)
{
	using param_load_from_file = bool(__thiscall*)(void*, void*, const char*, const char*, void*);
	static auto original_load_from_file = SEARCH(modules->client, signatures::other::load_from_file::signature()).cast<param_load_from_file>(); // @xref: "rb"
	assert(original_load_from_file != nullptr);
	return original_load_from_file(this, file_system, resource_name, path_id, evaluate_symbol_proc);
}

game_key_value* game_key_value::find(const char* key_name, const bool create)
{
	using param_find_key = game_key_value * (__thiscall*)(void*, const char*, bool);
	static auto original_find_key = SEARCH(modules->client, signatures::other::find_key::signature()).cast<param_find_key>();
	assert(original_find_key != nullptr);
	return original_find_key(this, key_name, create);
}

int game_key_value::get_int(const char* key_name, const int default_value)
{
	game_key_value* sub_key = this->find(key_name, false);

	if (sub_key == nullptr)
		return default_value;

	switch (sub_key->data_type)
	{
	case type_string:
		return std::atoi(sub_key->value);
	case type_wstring:
		return _wtoi(sub_key->wide_value);
	case type_float:
		return static_cast<int>(sub_key->float_value);
	case type_uint64:
		// can't convert, since it would lose data
		assert(false);
		return 0;
	default:
		break;
	}

	return sub_key->int_value;
}

float game_key_value::get_float(const char* key_name, const float default_value)
{
	game_key_value* sub_key = this->find(key_name, false);

	if (sub_key == nullptr)
		return default_value;

	switch (sub_key->data_type)
	{
	case type_string:
		return static_cast<float>(std::atof(sub_key->value));
	case type_wstring:
		return std::wcstof(sub_key->wide_value, nullptr);
	case type_float:
		return sub_key->float_value;
	case type_int:
		return static_cast<float>(sub_key->int_value);
	case type_uint64:
		return static_cast<float>(*reinterpret_cast<std::uint64_t*>(sub_key->value));
	case type_ptr:
	default:
		return 0.0f;
	}
}

const char* game_key_value::get_string(const char* key_name, const char* default_value)
{
	using param_get_string = const char* (__thiscall*)(void*, const char*, const char*);
	static auto original_get_string = SEARCH(modules->client, signatures::other::get_string::signature()).cast<param_get_string>();
	assert(original_get_string != nullptr);
	return original_get_string(this, key_name, default_value);
}

void game_key_value::set_string(const char* key_name, const char* string_value)
{
	game_key_value* sub_key = this->find(key_name, true);

	if (sub_key == nullptr)
		return;

	using param_set_string = void(__thiscall*)(void*, const char*);
	static auto original_set_string = SEARCH(modules->client, signatures::other::set_string::signature()).cast<param_set_string>();
	assert(original_set_string != nullptr);
	original_set_string(sub_key, string_value);
}

void game_key_value::set_int(const char* key_name, const int value)
{
	game_key_value* sub_key = this->find(key_name, true);

	if (sub_key == nullptr)
		return;

	sub_key->int_value = value;
	sub_key->data_type = type_int;
}

void game_key_value::set_uint64(const char* key_name, const int low_value, const int high_value)
{
	game_key_value* sub_key = this->find(key_name, true);

	if (sub_key == nullptr)
		return;

	// delete the old value.
	delete[] sub_key->value;

	// make sure we're not storing the WSTRING - as we're converting over to STRING.
	delete[] sub_key->wide_value;
	sub_key->wide_value = nullptr;

	sub_key->value = new char[sizeof(std::uint64_t)];
	*reinterpret_cast<std::uint64_t*>(sub_key->value) = static_cast<std::uint64_t>(high_value) << 32ULL | low_value;
	sub_key->data_type = type_uint64;
}