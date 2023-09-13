#include "console.h"

windows_console* console = new windows_console;

void windows_console::setup(const char* title)
{
	AllocConsole();
	freopen_s((FILE**)stdin, "conin$", "r", stdin);
	freopen_s((FILE**)stdout, "conout$", "w", stdout);
	SetConsoleTitle(title);
}

void windows_console::restore()
{
	fclose(stdin);
	fclose(stdout);
	FreeConsole();
}

void windows_console::log(log_type type, const char* logs, ...)
{
	char buffer[2048];
	if (std::strlen(logs) >= sizeof(buffer))
		return;

	va_list arguments;
	va_start(arguments, logs);
	vsprintf_s(buffer, sizeof(buffer), logs, arguments);
	va_end(arguments);

	std::array< std::string, log_type::log_count > output_type = { "[sdk]", "[+]", "[-]", "[!]" };
	std::cout << output_type[type].c_str() << " ";
	std::cout << buffer << std::endl;
}