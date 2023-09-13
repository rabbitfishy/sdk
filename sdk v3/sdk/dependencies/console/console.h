#pragma once
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <array>

enum log_type : int
{
	log_normal,
	log_success,
	log_fail,
	log_error,
	log_count
};

class windows_console
{
public:
	void setup(const char* title);
	void restore();
	void log(log_type type, const char* logs, ...);
};

extern windows_console* console;