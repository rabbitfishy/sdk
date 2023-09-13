#include "include.h"

// it has to be exactly 'DllMain' to tell the .dll file is the entry point to inject to the proccess.
#define DLL_ENTRY DllMain

DWORD WINAPI setup(HMODULE instance)
{
#ifdef _DEBUG
    console->setup("sdk console");
#endif // DEBUG

    __try {
        do {
            modules->setup();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        } // this the last module for the game to be loaded.
        while (!scanner->capture_module(modules->server_browser).hModule);

        // install game sdk.
        interfaces->setup();
        netvar->setup();
        render->setup();

        // install hooks.
        hooks->setup();
    }
    __except (dump->create_dump(GetExceptionInformation())) {
        FreeLibraryAndExitThread(instance, EXIT_FAILURE);
    }

    // unload hack cuz convenient!
    while (!GetAsyncKeyState(VK_END))
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

    FreeLibraryAndExitThread(instance, EXIT_SUCCESS);

    return FALSE;
}

DWORD WINAPI restore()
{
    // uninstall hooks.
    hooks->restore();

#ifdef _DEBUG
    console->restore();
#endif // DEBUG

    return TRUE;
}

BOOL APIENTRY DLL_ENTRY(HMODULE instance, DWORD call_reason, LPVOID reserved)
{
    switch (call_reason)
    {
    case DLL_PROCESS_ATTACH:
        // optimisation purposes.
        DisableThreadLibraryCalls(instance);

        if (const HANDLE thread = CreateThread(nullptr, NULL, (LPTHREAD_START_ROUTINE)setup, instance, NULL, nullptr))
            CloseHandle(thread);

        break;

    case DLL_PROCESS_DETACH:
        // perform any necessary cleanup.
        restore();
        break;
    }

    return TRUE;
}