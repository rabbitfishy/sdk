#include "../dependency.h"

windows_dump* dump = new windows_dump;

int windows_dump::create_dump(EXCEPTION_POINTERS* exception)
{
	MINIDUMP_EXCEPTION_INFORMATION info;

	WCHAR file_path[MAX_PATH];
	const WCHAR* file_name = L"sdk dump";
	WCHAR name_path[MAX_PATH];

	// get our file destination path (%temp% file).
	GetTempPathW(MAX_PATH, file_path);

	// name our folder name.
	StringCchPrintfW(name_path, MAX_PATH, L"%s%s", file_path, file_name);

	// create the file(s).
	CreateDirectoryW(name_path, NULL);

	// name our dump file(s).
	StringCchPrintfW(name_path, MAX_PATH, L"%s%s\\%s.dmp", file_path, file_name, L"sdk");

	// get handle to dump file.
	HANDLE dump_file = CreateFileW(name_path, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_WRITE | FILE_SHARE_READ, 0, CREATE_ALWAYS, 0, 0);

	info.ThreadId = GetCurrentThreadId();
	info.ExceptionPointers = exception;
	info.ClientPointers = TRUE;

	// check if we successfully created the dump file(s).
	bool successful = MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), dump_file, MiniDumpWithDataSegs, &info, NULL, NULL);

	// returns true if we created the dump otherwise we continue searching for any code that caused the crash.
	return successful ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH;
}

text_translate* translate = new text_translate;
game_global* csgo = new game_global;