#include "modelclass.h"

using namespace DirectX;

ModelClass::ModelClass()
    : m_vertex_buffer(nullptr),
    m_index_buffer(nullptr),
    m_vertex_count(0),
    m_index_count(0)
{
}

bool ModelClass::initialize(ID3D11Device *device)
{
    bool result;

    // Initialize vertex and index buffers
    result = initialize_buffers(device);
    if (!result)
    {
        return false;
    }
    
    return true;
}

void ModelClass::shutdown()
{
    shutdown_buffers();
}

void ModelClass::render(ID3D11DeviceContext *device_context)
{
    // Put the vertex and index buffers on the graphics pipeline to prepare them for drawing
    render_buffers(device_context);
}

int ModelClass::get_index_count()
{
    return m_index_count;
}

bool ModelClass::initialize_buffers(ID3D11Device *device)
{
    VertexType *vertices = nullptr;
    unsigned long *indices = nullptr;
    D3D11_BUFFER_DESC vertex_buffer_desc{}, index_buffer_desc{};
    D3D11_SUBRESOURCE_DATA vertex_data{}, index_data{};
    HRESULT result;

    // TEMP: Fill with triangle data
    m_vertex_count = 3;
    m_index_count = 3;

    vertices = new VertexType[m_vertex_count];
    if (!vertices)
    {
        // Out of memory? lol
        return false;
    }

    indices = new unsigned long[m_index_count];
    if (!indices)
    {
        return false;
    }

    // Bottom left
    vertices[0].position = XMFLOAT3(-1.0f, -1.0f, 0.0f); 
    vertices[0].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

    // Top middle
    vertices[1].position = XMFLOAT3(0.0f, 1.0f, 0.0f); 
    vertices[1].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

    // Bottom right
    vertices[2].position = XMFLOAT3(1.0f, -1.0f, 0.0f); 
    vertices[2].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

    indices[0] = 0; // Bottom left
    indices[1] = 1; // Top middle
    indices[2] = 2; // Bottom right

    // Config static vertex buffer
    vertex_buffer_desc.Usage = D3D11_USAGE_DEFAULT;
    vertex_buffer_desc.ByteWidth = sizeof(VertexType) * m_vertex_count;
    vertex_buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertex_buffer_desc.CPUAccessFlags = 0;
    vertex_buffer_desc.MiscFlags = 0;
    vertex_buffer_desc.StructureByteStride = 0;

    // Give the subresource structure a pointer to the vertex data
    vertex_data.pSysMem = vertices;
    vertex_data.SysMemPitch = 0;
    vertex_data.SysMemSlicePitch = 0;

    result = device->CreateBuffer(&vertex_buffer_desc, &vertex_data, &m_vertex_buffer);
    if (FAILED(result))
    {
        return false;
    }

    // Config static index buffer
    index_buffer_desc.Usage = D3D11_USAGE_DEFAULT;
    index_buffer_desc.ByteWidth = sizeof(unsigned long) * m_index_count;
    index_buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    index_buffer_desc.CPUAccessFlags = 0;
    index_buffer_desc.MiscFlags = 0;
    index_buffer_desc.StructureByteStride = 0;

    // Give the subresource structure a pointer to the vertex data
    index_data.pSysMem = indices;
    index_data.SysMemPitch = 0;
    index_data.SysMemSlicePitch = 0;

    result = device->CreateBuffer(&index_buffer_desc, &index_data, &m_index_buffer);
    if (FAILED(result))
    {
        return false;
    }

    // Arrays aren't needed anymore
    delete[] vertices;
    vertices = nullptr;

    delete[] indices;
    indices = nullptr;
    
    return true;
}

void ModelClass::shutdown_buffers()
{
    if (m_index_buffer)
    {
        m_index_buffer->Release();
        m_index_buffer = nullptr;
    }

    if (m_vertex_buffer)
    {
        m_vertex_buffer->Release();
        m_vertex_buffer = nullptr;
    }
}

void ModelClass::render_buffers(ID3D11DeviceContext *device_context)
{
    unsigned int stride = sizeof(VertexType);
    unsigned int offset = 0;

    // Set buffers as active
    device_context->IASetVertexBuffers(0, 1, &m_vertex_buffer, &stride, &offset);
    device_context->IASetIndexBuffer(m_index_buffer, DXGI_FORMAT_R32_UINT, 0);

    // Set the type of primitive that should be rendered from this vertex buffer
    // int in this case, triangle list
    device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}