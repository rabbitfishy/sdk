#include "netvar.h"
#include "../valve.h"

game_netvar* netvar = new game_netvar;

bool game_netvar::setup()
{
	// clear values.
	netvar_map.clear();

	for (auto data = interfaces->base_client->capture_all(); data != nullptr; data = data->next)
	{
		if (data->recv_table == nullptr)
			continue;

		this->store_props(data->network_name, data->recv_table);
	}

	console->log(log_type::log_success, "netvar installed!");

	return !netvar_map.empty();
}

void game_netvar::store_props(const char* name, receive_table* table, const std::uintptr_t offset, const int depth)
{
	const hash32 class_hash = HASH_RUNTIME(name);
	const hash32 delimiter_hash = HASH_RUNTIME("->", class_hash);
	const hash32 base_class_hash = HASH_RUNTIME("baseclass", delimiter_hash);

	for (int i = 0; i < table->prop_count; i++)
	{
		const auto prop = &table->props[i];

		// base tables filter.
		if (prop == nullptr || isdigit(prop->name[0]))
			continue;

		// concat to our netvar format just by hash.
		const hash32 total_hash = HASH_RUNTIME(prop->name, delimiter_hash);

		// skip baseclass.
		if (total_hash == base_class_hash)
			continue;

		// has child table.
		if (const auto child = prop->table; child != nullptr && child->prop_count > 0 && child->name[0] == 'D' && prop->type == dpt_data_table)
			this->store_props(name, child, static_cast<std::uintptr_t>(prop->offset) + offset, depth + 1);

		const std::uintptr_t total_offset = static_cast<std::uintptr_t>(prop->offset) + offset;

		// check if we have already grabbed property.
		if (!netvar_map[total_hash].offset)
		{
			// write values to map entry.
			netvar_map[total_hash] = { prop, total_offset };
		}
	}
}

std::uintptr_t game_netvar::find(game_data_map* map, const hash32 field_hash)
{
	while (map != nullptr)
	{
		for (int i = 0; i < map->fields; i++)
		{
			// skip it if we can't find the function name.
			if (map->descriptions[i].name == nullptr)
				continue;

			if (HASH_RUNTIME(map->descriptions[i].name) == field_hash)
				return map->descriptions[i].offset[data_offset_normal];

			if (map->descriptions[i].type == field_embedded)
			{
				if (map->descriptions[i].description != nullptr)
				{
					if (const auto offset = this->find(map->descriptions[i].description, field_hash); offset != 0U)
						return offset;
				}
			}
		}

		map = map->data_map;
	}

	return FALSE;
}