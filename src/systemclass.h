#pragma once
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include "inputclass.h"
#include "applicationclass.h"

class SystemClass
{
public:
    SystemClass();
    SystemClass(const SystemClass&) = default;
    ~SystemClass() = default;

    bool initialize();
    void shutdown();
    void run();

    LRESULT CALLBACK message_handler(HWND, UINT, WPARAM, LPARAM);

private:
    bool frame();
    void initialize_windows(int&, int&);
    void shutdown_windows();

    // LPCWSTR m_application_name;
    LPCSTR m_application_name;
    HINSTANCE m_hinstance;
    HWND m_hwnd;
    
    InputClass *m_input;
    ApplicationClass *m_application;
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

static SystemClass *application_handle = 0;