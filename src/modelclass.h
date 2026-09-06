#pragma once
#include <d3d11.h>
#include <DirectXMath.h>

class ModelClass
{
private:
    struct VertexType
    {
        DirectX::XMFLOAT3 position;
        DirectX::XMFLOAT4 color;
    };

    ID3D11Buffer *m_vertex_buffer;
    ID3D11Buffer *m_index_buffer;
    int m_vertex_count;
    int m_index_count;

    bool initialize_buffers(ID3D11Device *);
    void shutdown_buffers();
    void render_buffers(ID3D11DeviceContext *);

public:
    ModelClass();
    ModelClass(const ModelClass &) = default;
    ~ModelClass() = default;

    bool initialize(ID3D11Device *);
    void shutdown();
    void render(ID3D11DeviceContext *);

    int get_index_count();
};