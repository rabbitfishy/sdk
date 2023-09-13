#pragma once
#include <cstddef>
#include <cstdint>

enum send_prop_type : int
{
	dpt_int = 0,
	dpt_float,
	dpt_vector,
	dpt_vector_2d,
	dpt_string,
	dpt_array,
	dpt_data_table,
	dpt_int64,
	dpt_send_prop_type_max
};

struct receive_prop;

struct data_variant
{
	union {
		float			data_float;
		long			data_int;
		char*			data_string;
		void*			data;
		float			data_vector[3];
		std::int64_t	data_int64;
	};

	send_prop_type type;
};

class receive_proxy_data
{
public:
	const receive_prop*			prop;			// the property it's receiving.
	data_variant				value;			// the value given to you to store.
	int							element;		// which array element you're getting.
	int							object_id;		// the object being referred to.
};

using receive_variable_proxy	= void(__cdecl*)(const receive_proxy_data*, void*, void*);
using array_length_proxy		= void(__cdecl*)(void*, int, int);
using data_table_proxy			= void(__cdecl*)(const receive_prop*, void**, void*, int);

class standart_receive_proxies
{
public:
	receive_variable_proxy		int32_to_int8;
	receive_variable_proxy		int32_to_int16;
	receive_variable_proxy		int32_to_int32;
	receive_variable_proxy		int64_to_int64;
	receive_variable_proxy		float_to_float;
	receive_variable_proxy		vector_to_vector;
};

// receive data table.
struct receive_table
{
	receive_prop*		props;
	int					prop_count;
	void*				decoder;
	char*				name;
	bool				installed;
	bool				main_list;
};

// receive prop comes from the receive data table.
struct receive_prop
{
	char*					name;
	send_prop_type			type;
	int						flags;
	int						buffer_size;
	bool					inside;
	const void*				extra;
	receive_prop*			prop;
	array_length_proxy*		array_proxy;
	receive_variable_proxy	proxy;
	data_table_proxy*		table_proxy;
	receive_table*			table;
	int						offset;
	int						element_stride;
	int						elements;
	const char*				parent_name;
};