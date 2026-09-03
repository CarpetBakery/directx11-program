#include "d3dclass.h"

using namespace DirectX;

D3DClass::D3DClass()
    : m_swap_chain(nullptr),
      m_device(nullptr),
      m_device_context(nullptr),
      m_render_target_view(nullptr),
      m_depth_stencil_buffer(nullptr),
      m_depth_stencil_state(nullptr),
      m_depth_stencil_view(nullptr),
      m_raster_state(nullptr)
{
}

bool D3DClass::initialize(int screen_width, int screen_height, bool vsync, HWND hwnd, bool fullscreen, float screen_depth, float screen_near)
{
    HRESULT result;
    IDXGIFactory *factory;
    IDXGIAdapter *adapter;
    IDXGIOutput *adapter_output;
    unsigned int num_modes, i, numerator, denominator;
    unsigned long long string_length;
    DXGI_MODE_DESC *display_mode_list;
    DXGI_ADAPTER_DESC adapter_desc;
    int error;
    DXGI_SWAP_CHAIN_DESC swap_chain_desc;
    D3D_FEATURE_LEVEL feature_level;
    ID3D11Texture2D *back_buffer_ptr;
    D3D11_TEXTURE2D_DESC depth_buffer_desc;
    D3D11_DEPTH_STENCIL_DESC depth_stencil_sesc;
    D3D11_DEPTH_STENCIL_VIEW_DESC depth_stencil_view_desc;
    D3D11_RASTERIZER_DESC raster_desc;
    float fov, screen_aspect;

    // Store the vsync setting
    m_vsync_enabled = vsync;

    // Create a DirectX graphics interface factory
    result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void **)&factory);
    if (FAILED(result))
    {
        return false;
    }

    // Use factory to create and adapter for the primary interface (video card)
    result = factory->EnumAdapters(0, &adapter);
    if (FAILED(result))
    {
        return false;
    }

    // Enumerate the primary adapter output (monitor)
    result = adapter->EnumOutputs(0, &adapter_output);
    if (FAILED(result))
    {
        return false;
    }

    // Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display format for the adapter output (monitor)
    result = adapter_output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &num_modes, NULL);
    if (FAILED(result))
    {
        return false;
    }

    // Create a list to hold all the possible display modes for this monitor/video card combination
    display_mode_list = new DXGI_MODE_DESC[num_modes];
    if (!display_mode_list)
    {
        return false;
    }

    // Fill the display mode list structures
    result = adapter_output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &num_modes, display_mode_list);
    if (FAILED(result))
    {
        return false;
    }

    // Go through all the display modes and find the one that matches the screen width and height.
    // When a match is found, store the numerator and denominator of the refresh rate for that monitor
    for (int i = 0; i < num_modes; i++)
    {
        if (display_mode_list[i].Width == (unsigned int)screen_width &&
            display_mode_list[i].Height == (unsigned int)screen_height)
        {
            numerator = display_mode_list[i].RefreshRate.Numerator;
            denominator = display_mode_list[i].RefreshRate.Denominator;
        }
    }

    // Get the adapter (video card) description
    result = adapter->GetDesc(&adapter_desc);
    if (FAILED(result))
    {
        return false;
    }

    // Store the dedicated video memory in megabytes
    m_video_cardm_memory = (int)(adapter_desc.DedicatedVideoMemory / 1024 / 1024);

    // Convert the name of the video card to a character array and store it
    error = wcstombs_s(&string_length, m_video_card_description, 128, adapter_desc.Description, 128);
    if (error != 0)
    {
        return false;
    }
    
    // Release stuff related to getting info
    delete[] display_mode_list;
    display_mode_list = nullptr;

    adapter_output->Release();
    adapter_output = nullptr;

    adapter->Release();
    adapter = nullptr;

    factory->Release();
    factory = nullptr;

    // TODO: Initialize other stuff

    return true;
}