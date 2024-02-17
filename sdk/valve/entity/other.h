#pragma once
#include <assert.h>

#pragma region base handle definitions

// how many bits to use to encode an edict
#define	MAX_EDICT_BITS 11

// max number of edicts in a level
#define	MAX_EDICTS ( 1 << MAX_EDICT_BITS )

#define ENTITY_ENTRY_BITS ( MAX_EDICT_BITS + 2 )
#define ENTITY_ENTRIES ( 1 << ENTITY_ENTRY_BITS )

#define SERIAL_BITS 16
#define SERIAL_SHIFT_BITS ( 32 - SERIAL_BITS )
#define ENTITY_ENTRY_MASK ( ( 1 << SERIAL_BITS ) - 1 )

#define INVALID_HANDLE_INDEX 0xFFFFFFFF
#define STATIC_PROP_HANDLE_MASK ( ( 1 << 30 ) >> SERIAL_SHIFT_BITS )

#pragma endregion

class base_handle
{
public:
	base_handle() noexcept : index{ INVALID_HANDLE_INDEX } { }
	base_handle(const base_handle& other) noexcept : index{ other.index } { }

	base_handle(const int entry, const int serial) noexcept
	{
		assert(entry >= 0 && (entry & ENTITY_ENTRY_MASK) == entry);
		assert(serial >= 0 && serial < (1 << SERIAL_BITS));
		this->index = entry | (serial << SERIAL_SHIFT_BITS);
	}

	bool operator!=(const base_handle& other) const noexcept
	{
		return this->index != other.index;
	}

	bool operator==(const base_handle& other) const noexcept
	{
		return this->index == other.index;
	}

	bool operator<(const base_handle& other) const noexcept
	{
		return this->index < other.index;
	}

	[[nodiscard]] bool valid() const noexcept
	{
		return this->index != INVALID_HANDLE_INDEX;
	}

	[[nodiscard]] int entry_index() const noexcept
	{
		if (!this->valid())
			return ENTITY_ENTRIES - 1;

		return static_cast<int>(this->index & ENTITY_ENTRY_MASK);
	}

	[[nodiscard]] int serial_number() const noexcept
	{
		return static_cast<int>(this->index >> SERIAL_SHIFT_BITS);
	}

public:
	std::uint32_t index;
};

class handle_entity
{
public:
	virtual ~handle_entity() { }
	virtual void set_ref_entity_handle(const base_handle& ref) = 0;
	virtual const base_handle& get_ref_entity_handle() const = 0;
};

template <typename t>
class bit_flags
{
public:
	bit_flags() : value{ } { }
	bit_flags(const t& input) { value = input; }

	bool has(const t& input) const { return value & input; }
	void add(const t& input) { value |= input; }
	void remove(const t& input) { value &= ~input; }
	void clear() { value = NULL; }
	bool empty() const { return value == std::numeric_limits<t>::quiet_NaN(); }

	operator t() const { return value; }

	bit_flags<t>& operator=(const bit_flags<t>& input) {
		value = input.value;
		return *this;
	}

	t& operator=(const t& input) {
		value = input;
		return value;
	}

protected:
	t value;
};