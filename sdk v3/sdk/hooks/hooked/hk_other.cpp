#include "../hooks.h"
#include <intrin.h>
#pragma intrinsic(_ReturnAddress)

// credit: https://github.com/rollraw/qo0-base/blob/92a3b3aefa220da941c3feee387702c772d1af82/base/core/hooks.cpp#L197
void* __stdcall hk_functions::alloc_key_value_memory(int size)
{
	// return addresses of check function.
	static const std::uintptr_t alloc_key_value_engine = SEARCH(modules->engine, signatures::hooks::alloc_key_value_engine::signature()).add(0x1).rel32<std::uintptr_t>(0x4A);
	static const std::uintptr_t alloc_key_value_client = SEARCH(modules->client, signatures::hooks::alloc_key_value_client::signature()).add(0x1).rel32<std::uintptr_t>(0x3E);

	const std::uintptr_t return_address = reinterpret_cast<std::uintptr_t>(_ReturnAddress());

	if (return_address == alloc_key_value_engine || return_address == alloc_key_value_client)
		return nullptr;

	return originals->alloc_key_value_memory_original(interfaces->key_value_system, size);
}