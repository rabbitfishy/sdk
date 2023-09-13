#pragma once

class client_networkable;
using create_client_class = client_networkable * (__cdecl*)(int, int);
using create_event = client_networkable * (__cdecl*)();
struct receive_table;

struct game_client_class
{
	create_client_class			create_class;
	create_event				create_event;
	char*						network_name;
	receive_table*				recv_table;
	game_client_class*			next;
	int							class_id;
};

class game_base_client
{
public:
	game_client_class* capture_all() { return virtuals->call<game_client_class*>(this, 8); }
};