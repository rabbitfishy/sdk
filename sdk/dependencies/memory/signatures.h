#pragma once

#define PATTERN( name, pattern ) struct name { static const char* signature( ) { return ( const char* )( pattern ); } }

/*
* put all your pattern sigs here cuz its easier to find and maintaine
* example of use:
* struct [what ever name u want] <-- this here is only to keep it maintained and less messy also easier to find pattern(s) to update it in the future.
* {
*	pattern([sig name], "sig pattern here.");
* };
*/

namespace signatures
{
	struct interfaces
	{
		PATTERN(client_state, "A1 ? ? ? ? 8B 88 ? ? ? ? 85 C9 75 07");
		PATTERN(weapon_system, "8B 35 ? ? ? ? FF 10 0F B7 C0");
		PATTERN(input, "B9 ? ? ? ? F3 0F 11 04 24 FF 50 10");
		PATTERN(should_hit_entity, "55 8B EC 8B 55 0C 56 8B 75 08 57");
		PATTERN(should_hit_entity2, "55 8B EC 53 8B D9 56 57 8B 7D 08 8B 73 10");
	};

	struct hooks
	{
		PATTERN(alloc_key_value_engine, "E8 ? ? ? ? 83 C4 08 84 C0 75 10 FF 75 0C");
		PATTERN(alloc_key_value_client, "E8 ? ? ? ? 83 C4 08 84 C0 75 10");
	};

	struct entity
	{
		PATTERN(set_abs_origin, "55 8B EC 83 E4 F8 51 53 56 57 8B F1 E8");
		PATTERN(set_abs_angle, "55 8B EC 83 E4 F8 83 EC 64 53 56 57 8B F1 E8");
		PATTERN(animation_overlays, "8B 89 ? ? ? ? 8D 0C D1");
		PATTERN(lock_studio_hdr, "55 8B EC 51 53 8B D9 56 57 8D B3 ? ? ? ? FF");
	};

	struct other
	{
		PATTERN(key_value_constructor, "55 8B EC 56 8B F1 33 C0 8B 4D 0C 81");
		PATTERN(key_value_destructor, "56 8B F1 E8 ? ? ? ? 8B 4E 14");
		PATTERN(from_string, "55 8B EC 83 E4 F8 81 EC 0C 05");
		PATTERN(load_from_buffer, "55 8B EC 83 E4 F8 83 EC 34 53 8B 5D 0C 89");
		PATTERN(load_from_file, "55 8B EC 83 E4 F8 83 EC 14 53 56 8B 75 08 57 FF");
		PATTERN(find_key, "55 8B EC 83 EC 1C 53 8B D9 85 DB");
		PATTERN(get_string, "55 8B EC 83 E4 C0 81 EC ? ? ? ? 53 8B 5D 08");
		PATTERN(set_string, "55 8B EC A1 ? ? ? ? 53 56 57 8B F9 8B 08 8B 01");
		PATTERN(animation_state_constructor, "55 8B EC 56 8B F1 B9 ? ? ? ? C7 46");
		PATTERN(animation_state_update, "55 8B EC 83 E4 F8 83 EC 18 56 57 8B F9 F3 0F 11 54 24");
		PATTERN(animation_state_reset, "56 6A 01 68 ? ? ? ? 8B F1");
	};
}
