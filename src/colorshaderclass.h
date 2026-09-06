#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <fstream>

class ColorShaderClass
{
private:
    struct MatrixBufferType
    {
        DirectX::XMMATRIX world, view, projection;
    };

    ID3D11VertexShader* m_vertex_shader;
	ID3D11PixelShader* m_fragment_shader;
    ID3D11SamplerState *m_point_sampler;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matrix_buffer;

    bool initialize_shaders(ID3D11Device *, HWND, WCHAR *, WCHAR *);
    void shutdown_shader();
    void output_shader_error_message(ID3D10Blob *, HWND, WCHAR *);

    bool set_shader_parameters(ID3D11DeviceContext *, DirectX::XMMATRIX, DirectX::XMMATRIX, DirectX::XMMATRIX);
    void render_shader(ID3D11DeviceContext *, int);
    
public:
    ColorShaderClass();
    ColorShaderClass(const ColorShaderClass &) = default;
    ~ColorShaderClass() = default;

    bool initialize(ID3D11Device *, HWND);
    void shutdown();
    bool render(ID3D11DeviceContext *, int, const DirectX::XMMATRIX &, const DirectX::XMMATRIX &, const DirectX::XMMATRIX &);
};