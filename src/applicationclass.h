#pragma once
#include <Windows.h>

// -- Globals --
constexpr bool FULL_SCREEN = false;
constexpr bool VSYNC_ENABLED = true;
constexpr float SCREEN_DEPTH = 1000.0f;
constexpr float SCREEN_NEAR = 0.3f;

class ApplicationClass
{
public:
    ApplicationClass() = default;
    ApplicationClass(const ApplicationClass&) = default;
    ~ApplicationClass() = default;

    bool initialize(int, int, HWND);
    void shutdown();
    bool frame();
private:
    bool render();
};