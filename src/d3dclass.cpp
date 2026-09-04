#include "d3dclass.h"
#include "math.h"

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
    D3D11_DEPTH_STENCIL_DESC depth_stencil_desc;
    D3D11_DEPTH_STENCIL_VIEW_DESC depth_stencil_view_desc;
    D3D11_RASTERIZER_DESC raster_desc;
    float fov, screen_aspect;

    // Store the vsync setting
    m_vsync_enabled = vsync;

    // -- Get device info --
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

    // -- Initialize DirectX --
    // Initialize swapchain
    ZeroMemory(&swap_chain_desc, sizeof(swap_chain_desc));

    // Set to a single back buffer
    swap_chain_desc.BufferCount = 1;

    // Set width and height of backbuffer
    swap_chain_desc.BufferDesc.Width = screen_width;
    swap_chain_desc.BufferDesc.Height = screen_height;

    // Use 32-bit surface for backbuffer
    swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

    // Set refresh rate
    if (m_vsync_enabled)
    {
        swap_chain_desc.BufferDesc.RefreshRate.Numerator = numerator;
        swap_chain_desc.BufferDesc.RefreshRate.Denominator = denominator;
    }
    else
    {
        // Draw as fast as possible :(
        swap_chain_desc.BufferDesc.RefreshRate.Numerator = 0;
        swap_chain_desc.BufferDesc.RefreshRate.Denominator = 1;
    }

    swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swap_chain_desc.OutputWindow = hwnd;

    // Turn off multisampling
    swap_chain_desc.SampleDesc.Count = 1;
    swap_chain_desc.SampleDesc.Quality = 0;

    // Set to fullscreen or windowed mode
    swap_chain_desc.Windowed = !fullscreen;

    // Set scan line ordering and scaling to unspecified
    swap_chain_desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    swap_chain_desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

    // Discard back buffer contents after presenting
    swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    // Don't set the advanced flags
    swap_chain_desc.Flags = 0;

    // Use DirectX 11
    feature_level = D3D_FEATURE_LEVEL_11_0;

    // Create the swap chain, Direct3D device, and Direct3D device context
    result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &feature_level, 1,
                                           D3D11_SDK_VERSION, &swap_chain_desc, &m_swap_chain, &m_device, NULL, &m_device_context);
    if (FAILED(result))
    {
        // User might not have a card that supports DX11
        return false;
    }

    // Get pointer to back buffer
    result = m_swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID *)&back_buffer_ptr);
    if (FAILED(result))
    {
        return true;
    }

    // Create the render target view with the back buffer pointer
    result = m_device->CreateRenderTargetView(back_buffer_ptr, NULL, &m_render_target_view);
    if (FAILED(result))
    {
        return true;
    }

    // Don't need back buffer pointer anymore
    back_buffer_ptr->Release();
    back_buffer_ptr = nullptr;

    // Initialize the description of the depth buffer
    ZeroMemory(&depth_buffer_desc, sizeof(depth_buffer_desc));

    // Set up the description of the depth buffer
    depth_buffer_desc.Width = screen_width;
    depth_buffer_desc.Height = screen_height;
    depth_buffer_desc.MipLevels = 1;
    depth_buffer_desc.ArraySize = 1;
    depth_buffer_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depth_buffer_desc.SampleDesc.Count = 1;
    depth_buffer_desc.SampleDesc.Quality = 0;
    depth_buffer_desc.Usage = D3D11_USAGE_DEFAULT;
    depth_buffer_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depth_buffer_desc.CPUAccessFlags = 0;
    depth_buffer_desc.MiscFlags = 0;

    // Create texture for the depth buffer
    result = m_device->CreateTexture2D(&depth_buffer_desc, NULL, &m_depth_stencil_buffer);
    if (FAILED(result))
    {
        return true;
    }

    // Config depth stencil 
    ZeroMemory(&depth_stencil_desc, sizeof(depth_stencil_desc));

    depth_stencil_desc.DepthEnable = true;
    depth_stencil_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depth_stencil_desc.DepthFunc = D3D11_COMPARISON_LESS;

    depth_stencil_desc.StencilEnable = true;
    depth_stencil_desc.StencilReadMask = 0xFF;
    depth_stencil_desc.StencilWriteMask = 0xFF;

    // Stencil operations if pixel is front facing
    depth_stencil_desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depth_stencil_desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
    depth_stencil_desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    depth_stencil_desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    // Stencil operations if pixel is back facing
    depth_stencil_desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depth_stencil_desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
    depth_stencil_desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    depth_stencil_desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    result = m_device->CreateDepthStencilState(&depth_stencil_desc, &m_depth_stencil_state);
    if (FAILED(result))
    {
        return false;
    }

    // Bind depth stencil state
    m_device_context->OMSetDepthStencilState(m_depth_stencil_state, 1);

    // Config depth stencil view
    ZeroMemory(&depth_stencil_view_desc, sizeof(depth_stencil_view_desc));

    depth_stencil_view_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depth_stencil_view_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    depth_stencil_view_desc.Texture2D.MipSlice = 0;

    result = m_device->CreateDepthStencilView(m_depth_stencil_buffer, &depth_stencil_view_desc, &m_depth_stencil_view);
    if (FAILED(result))
    {
        return false;
    }

    // Bind the render target view and depth stencil buffer to the output render pipeline
    m_device_context->OMSetRenderTargets(1, &m_render_target_view, m_depth_stencil_view);

    // Config raster description, which will determine how and what polygons will be drawn
    raster_desc.AntialiasedLineEnable = false;
    raster_desc.CullMode = D3D11_CULL_BACK;
    raster_desc.DepthBias = 0;
    raster_desc.DepthBiasClamp = 0.0f;
    raster_desc.DepthClipEnable = true;
    raster_desc.FillMode = D3D11_FILL_SOLID;
    raster_desc.FrontCounterClockwise = false;
    raster_desc.MultisampleEnable = false;
    raster_desc.ScissorEnable = false;
    raster_desc.SlopeScaledDepthBias = 0.0f;

    result = m_device->CreateRasterizerState(&raster_desc, &m_raster_state);
    if (FAILED(result))
    {
        return false;
    }

    // Now set the rasterizer state
    m_device_context->RSSetState(m_raster_state);

    // Setup viewport
    m_viewport.Width = (float)screen_width;
    m_viewport.Height = (float)screen_height;
    m_viewport.MinDepth = 0.0f;
    m_viewport.MaxDepth = 1.0f;
    m_viewport.TopLeftX = 0.0f;
    m_viewport.TopLeftY = 0.0f;

    m_device_context->RSSetViewports(1, &m_viewport);

    // Create projection matrix
    fov = Math::pi / 4.0f;
    screen_aspect = (float)screen_width / (float)screen_height;
    m_projection_matrix = XMMatrixPerspectiveFovLH(fov, screen_aspect, screen_near, screen_depth);

    // Create world matrix
    m_world_matrix = XMMatrixIdentity();

    // TODO: Create view matrix

    // Create orthographic projection matrix for 2D rendering
    m_ortho_matrix = XMMatrixOrthographicLH((float)screen_width, (float)screen_height, screen_near, screen_depth);

    return true;
}

void D3DClass::shutdown()
{
    // Go windowed mode to avoid an exception when we release swap chain
    if (m_swap_chain)
    {
        m_swap_chain->SetFullscreenState(false, NULL);
    }

    if (m_raster_state)
    {
        m_raster_state->Release();
        m_raster_state = nullptr;
    }

    if (m_depth_stencil_view)
    {
        m_depth_stencil_view->Release();
        m_depth_stencil_view = nullptr;
    }

    if (m_depth_stencil_state)
    {
        m_depth_stencil_state->Release();
        m_depth_stencil_state = nullptr;
    }
    
    if (m_depth_stencil_buffer)
    {
        m_depth_stencil_buffer->Release();
        m_depth_stencil_buffer = nullptr;
    }

    if (m_render_target_view)
    {
        m_render_target_view->Release();
        m_render_target_view = nullptr;
    }

    if (m_device_context)
    {
        m_device_context->Release();
        m_device_context = nullptr;
    }

    if (m_device)
    {
        m_device->Release();
        m_device = nullptr;
    }

    if (m_swap_chain)
    {
        m_swap_chain->Release();
        m_swap_chain = nullptr;
    }
}

void D3DClass::begin_scene(float, float, float, float)
{
}

void D3DClass::end_scene()
{
}

ID3D11Device *D3DClass::get_device()
{
    return nullptr;
}

ID3D11DeviceContext *D3DClass::get_device_context()
{
    return nullptr;
}

void D3DClass::get_projection_matrix(DirectX::XMMATRIX &)
{
}

void D3DClass::get_world_matrix(DirectX::XMMATRIX &)
{
}

void D3DClass::get_ortho_matrix(DirectX::XMMATRIX &)
{
}

void D3DClass::get_video_card_info(char *, int &)
{
}

void D3DClass::set_back_buffer_render_target()
{
}

void D3DClass::reset_viewport()
{
}