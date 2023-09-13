#pragma once
#include <Windows.h>
#include <system_error>
#include "scanner.h"

class game_virtuals
{
public:
	// virtual function pointer of specified class at given index.
	template <typename t = void*>
	constexpr t get(void* thisptr, std::size_t index)
	{
		return (*static_cast<t**>(thisptr))[index];
	}

	// call virtual function of specified class at given index.
	// @note: references should be wrapped with std::ref call!
	template <typename t, typename ... param>
	constexpr t call(void* thisptr, std::size_t index, param... argument)
	{
		using virtual_detail = t(__thiscall*)(void*, decltype(argument)...);
		return (*static_cast<virtual_detail**>(thisptr))[index](thisptr, argument...);
	}
};

extern game_virtuals* virtuals;

// virtual protect raii wrapper
// credit: https://github.com/rollraw/qo0-base/blob/92a3b3aefa220da941c3feee387702c772d1af82/base/utilities/memory.h#L53
class game_protect
{
public:
	game_protect(void* base_address, const std::size_t address_length, const DWORD flags) : base(base_address), length(address_length)
	{
		if (!VirtualProtect(base_address, address_length, flags, &this->old_flags))
			throw std::system_error(GetLastError(), std::system_category(), "[!] failed to protect region!");
	}

	~game_protect()
	{
		VirtualProtect(this->base, this->length, this->old_flags, &this->old_flags);
	}

private:
	void*			base;
	std::size_t		length;
	DWORD			old_flags;
};

#define PROTECT( address, length, flags ) game_protect{ address, length, flags }

// thanks danielkrupinski, Bartis1313 & opaiidev! <3
// credit: https://github.com/opaiidev/airflow-legacy/blob/main/Airflow/base/tools/address.h
// ref: https://github.com/danielkrupinski/Osiris/blob/01e9e735056c4a8938a74f48bdf56f09d50de4fd/Source/SafeAddress.h#L6
// ref: https://github.com/Bartis1313/csgo/blob/0f7891bb72f11725dda584bc3c62a53d3bce0920/hack/gamememory/address.hpp#L21
enum dereference : std::size_t
{
	once = 1,
	twice,
	three
};

class game_address
{
public:
	explicit game_address() : base{ } { }
	~game_address() { }

	// pass by offset and pointer.
	explicit game_address(std::uintptr_t address) : base{ address } { }
	explicit game_address(const void* address) : base{ reinterpret_cast<std::uintptr_t>(address) } { }

	// arithmetic operators for native types.
	operator std::uintptr_t() { return this->base; }
	operator void* () { return reinterpret_cast<void*>(this->base); }
	operator const void* () { return reinterpret_cast<const void*>(this->base); }

	// is-equals-operator.
	bool operator==(game_address address) const { return this->cast<std::uintptr_t>() == address.cast<std::uintptr_t>(); }
	bool operator!=(game_address address) const { return this->cast<std::uintptr_t>() != address.cast<std::uintptr_t>(); }

	// cast pointer and add offset.
	template<typename c = game_address>
	c cast() const { return this->base ? *reinterpret_cast<c*>(this->base) : c{ }; }

	template<typename c = game_address>
	c cast(std::size_t offset) const { return this->base ? (*reinterpret_cast<c*>(this->base) + offset) : c{ }; }

	template<typename c = game_address>
	c cast(std::ptrdiff_t offset) const { return this->base ? (*reinterpret_cast<c*>(this->base) + offset) : c{ }; }

	// add offset.
	template<typename a = game_address>
	a add(std::size_t offset) const { return this->base ? (a)(this->base + offset) : a{ }; }

	template<typename a = game_address>
	a add(std::ptrdiff_t offset) const { return this->base ? (a)(this->base + offset) : a{ }; }

	// subtract offset.
	template<typename s = game_address>
	s sub(std::size_t offset) const { return this->base ? (s)(this->base - offset) : s{ }; }

	template<typename s = game_address>
	s sub(std::ptrdiff_t offset) const { return this->base ? (s)(this->base - offset) : s{ }; }

	// dereference.
	// verify adddress and dereference n times.
	template<typename d = game_address>
	d deref(std::size_t n = dereference::once)
	{
		std::uintptr_t result = { };

		if (!this->base)
			return d{ };

		result = this->base;

		for (std::size_t i = n; i > 0; i--)
		{
			// can't dereference, return null.
			if (!this->valid(result))
				return d{ };

			result = *reinterpret_cast<std::uintptr_t*>(result);
		}

		return (d)result;
	}

	// follow relative8 and relative16/32 offsets.
	template<typename r = game_address>
	r rel8(std::size_t offset)
	{
		std::uintptr_t result = { };
		std::uint8_t relative = { };

		if (!this->base)
			return r{ };

		result = this->base + offset;

		// get relative offset.
		relative = *reinterpret_cast<std::uint8_t*>(result);
		if (!relative)
			return r{ };

		// relative to address of next instruction.
		// short jumps can go forward and backward depending on the size of the second byte.
		// if the second byte is below 128, the jmp goes forwards.
		// if the second byte is above 128, the jmp goes backwards ( subtract two's complement of the relative offset from the address of the next instruction ).
		if (relative < 128)
			result = (result + 0x1) + relative;
		else
			result = (result + 0x1) - static_cast<std::uint8_t>(~relative + 0x1);

		return (r)result;
	}

	template<typename r = game_address>
	r rel32(std::size_t offset)
	{
		std::uintptr_t result = { };
		std::uint32_t relative = { };

		if (!this->base)
			return r{ };

		result = this->base + offset;

		// get relative offset.
		relative = *reinterpret_cast<std::uint32_t*>(result);
		if (!relative)
			return r{ };

		// relative to address of next instruction.
		result = (result + 0x4) + relative;

		return (r)result;
	}

	// set.
	template<typename s = std::uintptr_t>
	void set(const s& value)
	{
		if (!this->base)
			return;

		*reinterpret_cast<s*>(this->base) = value;
	}

private:
	// checks if address is not null and has correct page protection.
	static bool valid(std::uintptr_t address)
	{
		MEMORY_BASIC_INFORMATION info;

		// check for invalid address.
		if (!address)
			return false;

		// check for invalid page protection.
		if (!VirtualQuery((const void*)address, &info, sizeof(info)))
			return false;

		// todo - dex; fix this, its wrong... check for rwe or something too
		if ((info.Protect & PAGE_NOACCESS) || (info.Protect & PAGE_GUARD))
			return false;

		return true;
	}

	std::uintptr_t base;
};

#define SEARCH( modules, signatures ) game_address{ scanner->scan( modules, signatures ) }
#define ADDRESS( pointer ) game_address{ pointer }