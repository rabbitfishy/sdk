#pragma once
#include <Windows.h>
#include <string>
#include <bit>

/*
* fnv hash implementation.
* credit: http://www.isthe.com/chongo/tech/comp/fnv/
* fnv: https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function
*/

typedef std::uint32_t hash32;

namespace game_hash
{
	struct hash_internal
	{
		enum hash_seed : hash32
		{
			prime = 0x1000193,
			basis = 0x811C9DC5
		};
	};

	struct hashing : hash_internal
	{
		// compile time hash.
		static constexpr hash32 compile(const char* value, hash32 seed = hash_seed::basis)
		{
			return (value[0] == '\0') ? seed : compile(&value[1], (seed ^ static_cast<hash32>(value[0])) * hash_seed::prime);
		}

		// runtime hash.
		inline static hash32 runtime(const char* value, hash32 seed = hash_seed::basis)
		{
			for (std::size_t i = 0; i < strlen(value); i++)
			{
				seed ^= value[i];
				seed *= hash_seed::prime;
			}

			return seed;
		}
	};
}

using hash = game_hash::hashing;

#define HASH_COMPILE( value, ... ) hash::compile( value, __VA_ARGS__ )
#define HASH_RUNTIME( value, ... ) hash::runtime( value, __VA_ARGS__ )

/*
* fletcher checksum implementation (not implemented but still useful).
* credit: https://stackoverflow.com/a/71226758	// 32 bit
* credit: https://stackoverflow.com/a/34149107	// 8 bit
* -----------------------------------------------------------------------
* crc checksum implementation.
* credit: https://en.wikipedia.org/wiki/Cyclic_redundancy_check
* credit: https://github.com/rollraw/qo0-csgo/blob/master/base/sdk/hash/crc32.cpp
* credit: https://stackoverflow.com/a/26051190
*/

typedef unsigned long checksum_long;
typedef std::uint32_t checksum32;
typedef std::uint8_t checksum8;

namespace game_checksum
{
	struct checksum_internal
	{
		enum checksum_seed : checksum_long
		{
			polynomial	= 0xEDB88320,
			basis		= 0xFFFFFFFFUL
		};

		static constexpr void generate_table(checksum_long table[])
		{
			checksum_long remainder;
			checksum8 byte = 0;

			do {
				// start with data byte.
				remainder = byte;

				for (checksum_long bit = 8; bit > 0; bit--)
				{
					if (remainder & 1)
						remainder = (remainder >> 1) ^ checksum_seed::polynomial;
					else
						remainder = (remainder >> 1);
				}

				table[(checksum32)byte] = remainder;

			} while (0 != byte++);
		}

		static constexpr checksum_long proccess_crc(const checksum8* buffer, checksum32 buffer_size, checksum_long table[])
		{
			checksum_long crc = checksum_seed::basis;
			checksum32 i;

			for (i = 0; i < buffer_size; i++)
				crc = table[*buffer++ ^ (crc & 0xFF)] ^ (crc >> 8);

			return(~crc);
		}
	};

	struct crc_checksum : checksum_internal
	{
		// compile time crc.
		static constexpr checksum32 proccess(const void* buffer, checksum32 buffer_size)
		{
			// create our canvas table.
			checksum_long table[256];

			// first generate our lookup table and populate it.
			generate_table(table);

			// then proccess the generated lookup table.
			const checksum8* current_byte = static_cast<const checksum8*>(buffer);
			return proccess_crc(current_byte, buffer_size, table);
		}
	};
}

using checksum = game_checksum::crc_checksum;

#define CHECKSUM( crc, size ) checksum::proccess( &crc, size );