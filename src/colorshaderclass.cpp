#include "colorshaderclass.h"

using namespace DirectX;
using namespace std;

ColorShaderClass::ColorShaderClass()
    : m_vertex_shader(nullptr),
      m_fragment_shader(nullptr),
      m_layout(nullptr),
      m_matrix_buffer(nullptr)
{
}

bool ColorShaderClass::initialize(ID3D11Device *device, HWND hwnd)
{
    bool result;
    wchar_t vs_filename[128];
    wchar_t fs_filename[128];
    int error;

    // -- Set filenames of vs and fs --
    // TODO: Paths probably shouldn't be hardcoded
    error = wcscpy_s(vs_filename, 128, L"../../../data/color.vs.hlsl");
    if (error != 0)
    {
        return false;
    }

    error = wcscpy_s(fs_filename, 128, L"../../../data/color.fs.hlsl");
    if (error != 0)
    {
        return false;
    }

    result = initialize_shaders(device, hwnd, vs_filename, fs_filename);
    if (result)
    {
        return false;
    }

    return true;
}

void ColorShaderClass::shutdown()
{
    shutdown_shader();
}

bool ColorShaderClass::render(ID3D11DeviceContext *device_context, int index_count, const XMMATRIX &mat_world, const XMMATRIX &mat_view, const XMMATRIX &mat_proj)
{
    bool result;

    // Set shader parameters that it will use for rendering
    result = set_shader_parameters(device_context, mat_world, mat_view, mat_proj);
    if (result)
    {
        return false;
    }

    // Render the prepared buffers with the shader
    render_shader(device_context, index_count);

    return true;
}

bool ColorShaderClass::initialize_shaders(ID3D11Device *device, HWND hwnd, WCHAR *vs_filename, WCHAR *fs_filename)
{
    HRESULT result;
    ID3D10Blob *error_message = nullptr;
    ID3D10Blob *vertex_shader_buffer = nullptr;
    ID3D10Blob *fragment_shader_buffer = nullptr;
    D3D11_INPUT_ELEMENT_DESC polygon_layout[2];
    unsigned int num_elements;
    D3D11_BUFFER_DESC matrix_buffer_desc;

    // Compile vertex shader
    result = D3DCompileFromFile(vs_filename, NULL, NULL,
                                "ColorVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
                                &vertex_shader_buffer, &error_message);
    if (FAILED(result))
    {
        if (error_message)
        {
            // Shader compile error
            output_shader_error_message(error_message, hwnd, vs_filename);
        }
        else
        {
            // Couldn't find shader file
            MessageBox(hwnd, vs_filename, L"Missing shader file", MB_OK);
        }
        return false;
    }

    // Compile fragment shader
    result = D3DCompileFromFile(fs_filename, NULL, NULL,
        "ColorPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
        &fragment_shader_buffer, &error_message);
    if (FAILED(result))
    {
        if (error_message)
        {
            // Shader compile error
            output_shader_error_message(error_message, hwnd, fs_filename);
        }
        else
        {
            // Couldn't find shader file
            MessageBox(hwnd, fs_filename, L"Missing shader file", MB_OK);
        }
        return false;
    }

    // Create shader objects from buffers
    result = device->CreateVertexShader(vertex_shader_buffer->GetBufferPointer(), vertex_shader_buffer->GetBufferSize(), NULL, &m_vertex_shader);
    if (FAILED(result))
    {
        return false;
    }

    result = device->CreatePixelShader(fragment_shader_buffer->GetBufferPointer(), fragment_shader_buffer->GetBufferSize(), NULL, &m_fragment_shader);
    if (FAILED(result))
    {
        return false;
    }
    

    // TODO
    
    return true;
}