#include "colorshaderclass.h"
#include <string>

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
            // MessageBox(hwnd, vs_filename, "Missing shader file", MB_OK);
            MessageBox(hwnd, "Vertex shader", "Missing shader file", MB_OK);
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
            // MessageBox(hwnd, fs_filename, "Missing shader file", MB_OK);
            MessageBox(hwnd, "Fragment shader", "Missing shader file", MB_OK);
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

    // Create vertex input layout description
    // This setup needs to match the VertexType structure in the ModelClass and in the shader
    polygon_layout[0].SemanticName = "POSITION";
    polygon_layout[0].SemanticIndex = 0;
    polygon_layout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
    polygon_layout[0].InputSlot = 0;
    polygon_layout[0].AlignedByteOffset = 0;
    polygon_layout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    polygon_layout[0].InstanceDataStepRate = 0;

    polygon_layout[1].SemanticName = "COLOR";
    polygon_layout[1].SemanticIndex = 0;
    polygon_layout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    polygon_layout[1].InputSlot = 0;
    polygon_layout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
    polygon_layout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    polygon_layout[1].InstanceDataStepRate = 0;

    // Get a count of the elements in the layout
    num_elements = sizeof(polygon_layout) / sizeof(polygon_layout[0]);

    // Create the vertex input layout
    result = device->CreateInputLayout(polygon_layout,
                                       num_elements,
                                       vertex_shader_buffer->GetBufferPointer(),
                                       vertex_shader_buffer->GetBufferSize(),
                                       &m_layout);
    if (FAILED(result))
    {
        return false;
    }

    // Vertex/fragment shader buffer aren't needed anymore
    vertex_shader_buffer->Release();
    vertex_shader_buffer = nullptr;

    fragment_shader_buffer->Release();
    fragment_shader_buffer = nullptr;

    // Config dynamic matrix constant buffer that is in the vertex shader
    matrix_buffer_desc.Usage = D3D11_USAGE_DYNAMIC; // Vertex data is updated every frame
    matrix_buffer_desc.ByteWidth = sizeof(MatrixBufferType);
    matrix_buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    matrix_buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    matrix_buffer_desc.MiscFlags = 0;
    matrix_buffer_desc.StructureByteStride = 0;

    // Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class
    result = device->CreateBuffer(&matrix_buffer_desc, NULL, &m_matrix_buffer);
    if (FAILED(result))
    {
        return false;
    }

    return true;
}

void ColorShaderClass::shutdown_shader()
{
    if (m_matrix_buffer)
    {
        m_matrix_buffer->Release(); 
        m_matrix_buffer = nullptr;
    }

    if (m_layout)
    {
        m_layout->Release(); 
        m_layout = nullptr;
    }

    if (m_fragment_shader)
    {
        m_fragment_shader->Release(); 
        m_fragment_shader = nullptr;
    }

    if (m_vertex_shader)
    {
        m_vertex_shader->Release(); 
        m_vertex_shader = nullptr;
    }
}

void ColorShaderClass::output_shader_error_message(ID3D10Blob *error_message, HWND hwnd, WCHAR *shader_filename)
{
    char *compile_errors = nullptr;
    unsigned long long buffer_size, i;
    ofstream fout;

    compile_errors = (char *)(error_message->GetBufferPointer());
    buffer_size = error_message->GetBufferSize();

    // Write errors to file
    fout.open("shader_error.txt");
    for (i = 0; i < buffer_size; i++)
    {
        fout << compile_errors[i];
    }
    fout.close();

    error_message->Release();
    error_message = nullptr;

    // TODO: Find out how to convert WCHAR to LPCSTR
    // MessageBox(hwnd, "Error compiling shader. Check shader_error.txt for message.", shader_filename, MB_OK);
    MessageBox(hwnd, "Error compiling shader. Check shader_error.txt for message.", "Shader error", MB_OK);
}

bool ColorShaderClass::set_shader_parameters(ID3D11DeviceContext *device_context, DirectX::XMMATRIX mat_world, DirectX::XMMATRIX mat_view, DirectX::XMMATRIX mat_proj)
{
    HRESULT result;
    D3D11_MAPPED_SUBRESOURCE mapped_resource;
    MatrixBufferType *data_ptr;
    unsigned int buffer_number;

    // DX11 requires matrices be transposed
    mat_world = XMMatrixTranspose(mat_world);
    mat_view = XMMatrixTranspose(mat_view);
    mat_proj = XMMatrixTranspose(mat_proj);

    // Need to lock matrix buffer to set matrices
    result = device_context->Map(m_matrix_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource);
    if (FAILED(result))
    {
        return false;
    }

    // Copy matrices into constant buffer
    data_ptr = (MatrixBufferType *)mapped_resource.pData;
    data_ptr->world = mat_world;
    data_ptr->view = mat_view;
    data_ptr->projection = mat_proj;

    // Unlock the constant buffer
    device_context->Unmap(m_matrix_buffer, 0);

    // Set the position of the constant buffer in the vertex shader
    buffer_number = 0;
    
    // Set the constant buffer in the vertex shader with the updated values
    device_context->VSSetConstantBuffers(buffer_number, 1, &m_matrix_buffer);

    return true;
}

void ColorShaderClass::render_shader(ID3D11DeviceContext *device_context, int index_count)
{
    // Set the vertex input layout
    device_context->IASetInputLayout(m_layout);

    // Set the vertex and pixel shaders that will be used to render this triangle
    device_context->VSSetShader(m_vertex_shader, NULL, 0);
    device_context->PSSetShader(m_fragment_shader, NULL, 0);

    // Render the triangle
    device_context->DrawIndexed(index_count, 0, 0);
}