#include "systemclass.h"
#include "debug.h"

namespace
{
    constexpr int SCREEN_WIDTH = 800;
    constexpr int SCREEN_HEIGHT = 600;
}

SystemClass::SystemClass()
{
    m_input = nullptr;
    m_application = nullptr;
}

bool SystemClass::initialize()
{
    int screen_width = 0;
    int screen_height = 0;
    bool result;

    initialize_windows(screen_width, screen_height);

    m_input = new InputClass();
    m_input->initialize();

    m_application = new ApplicationClass();
    result = m_application->initialize(screen_width, screen_height, m_hwnd);
    if (!result)
    {
        return false;
    }

    DebugConsole::init(m_hwnd, screen_width, screen_height);

    return true;
}

void SystemClass::shutdown()
{
    DebugConsole::free();
    
    if (m_application)
    {
        m_application->shutdown();
        delete m_application;
        m_application = nullptr;
    }

    if (m_input)
    {
        delete m_input;
        m_input = nullptr;
    }

    shutdown_windows();
}

void SystemClass::run()
{
    // -- Main loop --
    MSG msg;
    bool done, result;

    ZeroMemory(&msg, sizeof(MSG));

    // Handle messages
    done = false;
    while (!done)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        if (msg.message == WM_QUIT)
        {
            done = true;
            break;
        }
        else
        {
            // Do frame processing
            result = frame();
            if (!result)
            {
                done = true;
            }
        }
    }
}

bool SystemClass::frame()
{
    if (m_input->is_key_down(VK_ESCAPE))
    {
        return false;
    }

    // Do frame processing
    return m_application->frame();
}

// Where we direct our windows messages into
LRESULT CALLBACK SystemClass::message_handler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
    switch (umsg)
    {
    case WM_KEYDOWN:
        // If a key is pressed, send it to the input object
        m_input->key_down((unsigned int)wparam);
        return 0;

    case WM_KEYUP:
        // If a key is pressed, send it to the input object
        m_input->key_up((unsigned int)wparam);
        return 0;

    // Any other messages send to the default message handler as our application won't make use of them
    default:
        return DefWindowProc(hwnd, umsg, wparam, lparam);
    }
}

void SystemClass::initialize_windows(int &screen_width, int &screen_height)
{
    WNDCLASSEX wc{};
    DEVMODE dm_screen_settings;
    int posx, posy;

    // Get an external pointer to this object(?)
    application_handle = this;

    // Get the instance of this application
    m_hinstance = GetModuleHandle(NULL);

    // Give the application a name
    m_application_name = "DX11 Program";

    // Setup windows class with default settings
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = m_hinstance;
    wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
    wc.hIconSm = wc.hIcon;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = m_application_name;
    wc.cbSize = sizeof(WNDCLASSEX);

    // Register window class
    RegisterClassEx(&wc);

    screen_width = GetSystemMetrics(SM_CXSCREEN);
    screen_height = GetSystemMetrics(SM_CYSCREEN);

    // Setup the screen settings
    if (FULL_SCREEN)
    {
        // If fullscreen, set the screen to maximum size of user's desktop
        memset(&dm_screen_settings, 0, sizeof(dm_screen_settings));
        dm_screen_settings.dmSize = sizeof(dm_screen_settings);
        dm_screen_settings.dmPelsWidth = (unsigned long)screen_width;
        dm_screen_settings.dmPelsHeight = (unsigned long)screen_height;
        dm_screen_settings.dmBitsPerPel = 32;
        dm_screen_settings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

        // Change the display settings to fullscreen
        ChangeDisplaySettings(&dm_screen_settings, CDS_FULLSCREEN);

        // Set the position of the window to the top left corner
        posx = 0;
        posy = 0;
    }
    else
    {
        // Windowed mode
        screen_width = SCREEN_WIDTH;
        screen_height = SCREEN_HEIGHT;

        // Place window in the middle of the screen
        posx = (GetSystemMetrics(SM_CXSCREEN) - screen_width) / 2;
        posy = (GetSystemMetrics(SM_CYSCREEN) - screen_height) / 2;
    }

    // Create window with screen settings and get handle
    m_hwnd = CreateWindowEx(
        WS_EX_APPWINDOW,
        m_application_name, m_application_name,
        // WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
        WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        posx, posy, screen_width, screen_height, NULL, NULL, m_hinstance, NULL);

    // Bring the window up on the screen and set it as main focus
    ShowWindow(m_hwnd, SW_SHOW);
    SetForegroundWindow(m_hwnd);
    SetFocus(m_hwnd);

    // Hide mouse cursor
    ShowCursor(false);
}

void SystemClass::shutdown_windows()
{
	// Show the mouse cursor.
	ShowCursor(true);

	// Fix the display settings if leaving full screen mode.
	if(FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// Remove the window.
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	// Remove the application instance.
	UnregisterClass(m_application_name, m_hinstance);
	m_hinstance = NULL;

	// Release the pointer to this class.
	application_handle = NULL;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch(umessage)
	{
		// Check if the window is being destroyed.
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}

		// Check if the window is being closed.
		case WM_CLOSE:
		{
			PostQuitMessage(0);		
			return 0;
		}

		// All other messages pass to the message handler in the system class.
		default:
		{
			return application_handle->message_handler(hwnd, umessage, wparam, lparam);
		}
	}
}
