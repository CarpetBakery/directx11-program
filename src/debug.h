#pragma once
#include <iostream>

#ifdef BUILD_PLATFORM_WINDOWS
#include <Windows.h>

#ifdef _DEBUG

// -- Debug assert --
#define LB_ASSERT(condition, msg)                                               \
    do                                                                          \
    {                                                                           \
        if (!(condition))                                                       \
        {                                                                       \
            std::cout << "Error in '" << __FILE__ << "' at line #" << __LINE__; \
            std::cout << "\n  --> " << msg << "\n";                             \
            MessageBox(                                                         \
                NULL,                                                           \
                msg,                                                            \
                "Error",                                                        \
                MB_OK | MB_ICONERROR);                                          \
            __debugbreak();                                                     \
        }                                                                       \
    } while (0)

#else // _DEBUG
#define LB_ASSERT(condition, msg)                   \
    do                                              \
    {                                               \
        if (!(condition))                           \
        {                                           \
            std::cout << "\n  --> " << msg << "\n"; \
        }                                           \
    } while (0)

#endif // _DEBUG

namespace DebugConsole
{
    inline bool shown = false;

    // Create and initialize new console window
    void init(HWND hwnd_main, int screen_width, int screen_height);
    // Destroy debug console window
    void free();

    // Show the debug console window
    void show();
    // Hide the debug console window
    void hide();
}

#endif // BUILD_PLATFORM_WINDOWS
