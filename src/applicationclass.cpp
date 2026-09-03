#include "applicationclass.h"

bool ApplicationClass::initialize(int screen_width, int screen_height, HWND hwnd)
{
    bool result;

    m_direct3d = new D3DClass();
    result = m_direct3d->initialize(screen_width, screen_height, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
    if (!result)
    {
        return false;
    }
    
    return true;
}

void ApplicationClass::shutdown()
{
    if (m_direct3d)
    {
        m_direct3d->shutdown();
        delete m_direct3d;
        m_direct3d = nullptr;
    }
}

bool ApplicationClass::frame()
{
    bool result;

    // Render the scene
    result = render();
    if (!result)
    {
        return false;
    }
    
    return true;
}

bool ApplicationClass::render()
{
    // CLear buffer
    m_direct3d->begin_scene(0.5f, 0.5f, 0.5f, 1.0f);

    // Present rendered scene to screen
    m_direct3d->end_scene();

    return true;
}
