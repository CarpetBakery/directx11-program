#pragma once
#include <DirectXMath.h>

class CameraClass
{
private:
    float m_position_x, m_position_y, m_position_z;
    float m_rotation_x, m_rotation_y, m_rotation_z;
    DirectX::XMMATRIX m_mat_view;
    
public:
    CameraClass();
    CameraClass(const CameraClass &) = default;
    ~CameraClass() = default;

    void set_position(float, float, float);
    void set_rotation(float, float, float);

    DirectX::XMFLOAT3 get_position() const;
    DirectX::XMFLOAT3 get_rotation() const;

    void render();
    void get_view_matrix(DirectX::XMMATRIX &);
};