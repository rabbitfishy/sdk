#pragma once

typedef DWORD base_handle;

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