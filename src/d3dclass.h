#pragma once
// #pragma comment(lib, "d3d11.lib")
// #pragma comment(lib, "dxgi.lib")
// #pragma comment(lib, "d3dcompiler.lib")

#include <d3d11.h>
#include <DirectXMath.h>

class D3DClass
{
public:
    D3DClass();
    D3DClass(const D3DClass &) = default;
    ~D3DClass() = default;

    bool initialize(int, int, bool, HWND, bool, float, float);
    void shutdown() {};

    void begin_scene(float, float, float, float) {};
    void end_scene() {};

    ID3D11Device *get_device() {};
    ID3D11DeviceContext *get_device_context() {};

    void get_projection_matrix(DirectX::XMMATRIX &) {};
    void get_world_matrix(DirectX::XMMATRIX &) {};
    void get_ortho_matrix(DirectX::XMMATRIX &) {};

    void get_video_card_info(char *, int &) {};

    void set_back_buffer_render_target() {};
    void reset_viewport() {};

private:
    bool m_vsync_enabled;
    int m_video_cardm_memory;
    char m_video_card_description[128];
    IDXGISwapChain *m_swap_chain;
    ID3D11Device *m_device;
    ID3D11DeviceContext *m_device_context;
    ID3D11RenderTargetView *m_render_target_view;
    ID3D11Texture2D *m_depth_stencil_buffer;
    ID3D11DepthStencilState *m_depth_stencil_state;
    ID3D11DepthStencilView *m_depth_stencil_view;
    ID3D11RasterizerState *m_raster_state;
    DirectX::XMMATRIX m_projection_matrix;
    DirectX::XMMATRIX m_world_matrix;
    DirectX::XMMATRIX m_ortho_matrix;
    D3D11_VIEWPORT m_viewport;
};