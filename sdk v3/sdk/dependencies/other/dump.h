#pragma once
#include <Windows.h>
#include <tchar.h>
#include <strsafe.h>
#include <iostream>

#include <DbgHelp.h>
#pragma comment(lib, "DbgHelp.lib")

/*
* mini dumps can help you debug with better accuracy,
* as mini dumps tells you exactly which line of code caused
* the crash by the program.
*
* these helped me figure out how to create the dumps.
* credit: https://www.youtube.com/watch?v=W2NnpEZ1Gdk
*/

class windows_dump
{
public:
	int create_dump(EXCEPTION_POINTERS* exception);
};

extern windows_dump* dump;