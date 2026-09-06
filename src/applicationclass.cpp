#include "applicationclass.h"

using namespace DirectX;

ApplicationClass::ApplicationClass()
    : m_direct3d(nullptr),
      m_cam(nullptr),
      m_model(nullptr),
      m_color_shader(nullptr)
{
}

bool ApplicationClass::initialize(int screen_width, int screen_height, HWND hwnd)
{
    bool result;

    m_direct3d = new D3DClass();
    result = m_direct3d->initialize(screen_width, screen_height, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
    if (!result)
    {
        MessageBox(hwnd, "Could not initialize Direct3D.", "Error", MB_OK);
        return false;
    }

    // Create objects
    m_cam = new CameraClass();
    m_cam->set_position(0.0f, 0.0f, -5.0f);

    m_model = new ModelClass();
    result = m_model->initialize(m_direct3d->get_device());
    if (!result)
    {
        MessageBox(hwnd, "Could not initialize model object.", "Error", MB_OK);
        return false;
    }

    m_color_shader = new ColorShaderClass();
    result = m_color_shader->initialize(m_direct3d->get_device(), hwnd);
    if (!result)
    {
        MessageBox(hwnd, "Could not initialize the color shader object.", "Error", MB_OK);
        return false;
    }

    return true;
}

void ApplicationClass::shutdown()
{
    // Release objects
    if (m_color_shader)
    {
        m_color_shader->shutdown();
        delete m_color_shader;
        m_color_shader = nullptr;
    }

    if (m_model)
    {
        m_model->shutdown();
        delete m_model;
        m_model = nullptr;
    }

    if (m_cam)
    {
        delete m_cam;
        m_cam = nullptr;
    }

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
    XMMATRIX mat_world, mat_view, mat_proj;
    bool result;

    // Clear buffers
    m_direct3d->begin_scene(0.0f, 0.0f, 0.0f, 1.0f);

    // Generate the view matrix based on the camera's position
    m_cam->render();

    // Get copies of matrices
    m_direct3d->get_world_matrix(mat_world);
    m_cam->get_view_matrix(mat_view);
    m_direct3d->get_projection_matrix(mat_proj);

    // Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing
    m_model->render(m_direct3d->get_device_context());

    // Render the model using the color shader
    result = m_color_shader->render(
        m_direct3d->get_device_context(),
        m_model->get_index_count(),
        mat_world, mat_view, mat_proj);
    if (!result)
    {
        return false;
    }

    // Present the rendered scene to the screen
    m_direct3d->end_scene();

    return true;
}
