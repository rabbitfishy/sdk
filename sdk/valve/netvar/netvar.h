#pragma once
#include "data_map.h"
#include "data_table.h"
#include "../../dependencies/other/hash.h"
#include <unordered_map>
#include <type_traits>

class game_netvar
{
public:
	struct netvar_object
	{
		receive_prop*	prop	= nullptr;
		std::uintptr_t	offset	= 0U;
	};

	bool setup();

	/*
	* stores the variables of objects in the hierarchy.
	* used to iterate through an object's data descriptions from data map.
	*/
	std::uintptr_t find(game_data_map* map, const hash32 field_hash);

	std::unordered_map<hash32, netvar_object> netvar_map = { };
private:
	/*
	* recursively stores networked properties info from data tables in our map
	* and also format our dump and write values to file.
	*/
	void store_props(const char* name, receive_table* table, const std::uintptr_t offset = 0U, const int depth = 0);
};

extern game_netvar* netvar;

#pragma region netvar methods

#define NETVAR_OFFSET( table, add, name, type )																\
	[[nodiscard]] std::add_lvalue_reference_t<type> name()													\
	{																										\
		static constexpr std::uint32_t hashed = HASH_COMPILE(table);										\
		static std::uintptr_t map_offset = netvar->netvar_map[hashed].offset;								\
		return *(std::add_pointer_t<type>)(reinterpret_cast<std::uintptr_t>(this) + map_offset + add);		\
	}

#define NETVAR( table, name, type ) NETVAR_OFFSET( table, 0U, name, type )

#define NETVAR_OFFSET_PTR( table, add, name, type )															\
	[[nodiscard]] std::add_pointer_t<type> name()															\
	{																										\
		static constexpr std::uint32_t hashed = HASH_COMPILE(table);										\
		static std::uintptr_t map_offset = netvar->netvar_map[hashed].offset;								\
		return (std::add_pointer_t<type>)(reinterpret_cast<std::uintptr_t>(this) + map_offset + add);		\
	}

#define NETVAR_PTR( table, name, type ) NETVAR_OFFSET_PTR( table, 0U, name, type )

#define DATAMAP( map, table, name, type )																	\
	[[nodiscard]] std::add_lvalue_reference_t<type> name()													\
	{																										\
		static constexpr std::uint32_t hashed = HASH_COMPILE(table);										\
		static std::uintptr_t map_offset = netvar->find(map, hashed);										\
		return *(std::add_pointer_t<type>)(reinterpret_cast<std::uintptr_t>(this) + map_offset);			\
	}

#define DATAMAP_PTR( map, table, name, type )																\
	[[nodiscard]] std::add_pointer_t<type> name()															\
	{																										\
		static constexpr std::uint32_t hashed = HASH_COMPILE(table);										\
		static std::uintptr_t map_offset = netvar->find(map, hashed);										\
		return (std::add_pointer_t<type>)(reinterpret_cast<std::uintptr_t>(this) + map_offset);				\
	}

#define OFFSET( name, type, offset )																		\
	[[nodiscard]] std::add_lvalue_reference_t<type> name()													\
	{																										\
		return *(std::add_pointer_t<type>)(reinterpret_cast<std::uintptr_t>(this) + offset);				\
	}

#define OFFSET_PTR( name, type, offset )																	\
	[[nodiscard]] std::add_pointer_t<type> name()															\
	{																										\
		return (std::add_pointer_t<type>)(reinterpret_cast<std::uintptr_t>(this) + offset);					\
	}

#pragma endregion