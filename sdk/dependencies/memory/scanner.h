#pragma once
#include <TlHelp32.h>

#define in_range( x, a, b ) ( x >= a && x <= b )
#define data_bits( x ) ( in_range( ( x & ( ~0x20 ) ), 'A', 'F' ) ? ( ( x & ( ~0x20 ) ) - 'A' + 0xA ): ( in_range( x, '0', '9' ) ? x - '0': 0 ) )
#define data_byte( x ) ( data_bits( x[0] ) << 4 | data_bits( x[1] ) )

class game_scanner
{
public:
	MODULEENTRY32 capture_module(const char* modules);
	DWORD scan(const char* modules, const char* signature);
};

extern game_scanner* scanner;