#pragma once
#include <Windows.h>
#include <corecrt_malloc.h>
#include <malloc.h>
#include <vcruntime_string.h>

template < class t, class n = int >
class utl_memory
{
public:
	t& operator[](n i) { return memory[i]; }
	const t& operator[](n i) const { return memory[i]; }

	t* base() { return memory; }

	[[nodiscard]] int allocated_count() const { return allocation_count; }
	[[nodiscard]] bool is_externally_allocated() const { return grow_size < 0; }

	void grow(const int count = 1)
	{
		if (is_externally_allocated())
			return;

		int allocation_requested = allocation_count + count;
		int new_allocation_count = 0;

		if (grow_size)
			allocation_count = ((1 + ((allocation_requested - 1) / grow_size)) * grow_size);
		else
		{
			if (!allocation_count)
				allocation_count = (31 + sizeof(t)) / sizeof(t);

			while (allocation_count < allocation_requested)
				allocation_count <<= 1;
		}

		if (static_cast<int>(static_cast<n>(new_allocation_count)) < allocation_requested)
		{
			if (static_cast<int>(static_cast<n>(new_allocation_count)) == 0 && static_cast<int>(static_cast<n>(new_allocation_count - 1)) >= allocation_requested)
				--new_allocation_count;
			else
			{
				if (static_cast<int>(static_cast<n>(allocation_requested)) != allocation_requested)
					return;

				while (static_cast<int>(static_cast<n>(new_allocation_count)) < allocation_requested)
					new_allocation_count = (new_allocation_count + allocation_requested) / 2;
			}
		}

		allocation_count = new_allocation_count;

		if (memory != nullptr)
			memory = static_cast<t*>(std::realloc(memory, allocation_count * sizeof(t)));
		else
			memory = static_cast<t*>(std::malloc(allocation_count * sizeof(t)));
	}

protected:
	t*		memory;
	int		allocation_count;
	int		grow_size;
};

template <class t>
void destruct(t* memory) { memory->~t(); }

template <class t>
t* construct(t* memory) { return new(memory) t; }

template< class t >
t* copy(t* memory, t const& source) { return new(memory) t(source); }

template < class t, class a = utl_memory< t > >
class utl_vector
{
	using allocator = a;
public:
	auto begin() noexcept { return memory.base(); }
	auto end() noexcept { return memory.base() + size_count; }

	auto begin() const noexcept { return memory.base(); }
	auto end() const noexcept { return memory.base() + size_count; }

	t& operator[](int i) { return memory[i]; }
	const t& operator[](int i) const { return memory[i]; }

	t& element(int i) { return memory[i]; }
	const t& element(int i) const { return memory[i]; }

	t* base() { return memory.base(); }

	int count() const { return size_count; }
	int& size() { return size_count; }

	void grow_vector(int count = 1)
	{
		if (size_count + count > memory.allocated_count())
			memory.grow(size_count + count - memory.allocated_count());

		size_count += count;
	}

	void shift_elements_right(const int element_index, const int shift = 1)
	{
		const int move_count = size_count - element_index - shift;

		if (move_count > 0 && shift > 0)
			std::memmove(&element(element_index + shift), &element(element_index), move_count * sizeof(t));
	}

	void shift_elements_left(const int element_index, const int shift = 1)
	{
		const int move_count = size_count - element_index - shift;

		if (move_count > 0 && shift > 0)
			std::memmove(&element(element_index), &element(element_index + shift), move_count * sizeof(t));
	}

	int insert_before(const int element_index)
	{
		// can insert at the end
		grow_vector();
		shift_elements_right(element_index);
		construct(&element(element_index));
		return element_index;
	}

	int insert_before(int element_index, const t& source)
	{
		// reallocate if can't insert something that's in the list
		// can insert at the end
		grow_vector();
		shift_elements_right(element_index);
		copy(&element(element_index), source);
		return element_index;
	}

	int add_tail() { return insert_before(size_count); }
	int add_tail(const t& source) { return insert_before(size_count, source); }

	int find(const t& source) const
	{
		for (int i = 0; i < count(); ++i)
		{
			if (element(i) == source)
				return i;
		}

		return -1;
	}

	void remove(const int element_index)
	{
		destruct(&element(element_index));
		shift_elements_left(element_index);
		--size_count;
	}

	void remove_all()
	{
		for (int i = size_count; --i >= 0;)
			destruct(&element(i));

		size_count = 0;
	}

	bool find_remove(const t& source)
	{
		if (const int element_index = find(source); element_index != -1)
		{
			remove(element_index);
			return true;
		}

		return false;
	}

protected:
	allocator	memory;
	int			size_count;
	t*			elements;
};