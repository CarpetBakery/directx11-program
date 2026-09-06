#include "cameraclass.h"

using namespace DirectX;

CameraClass::CameraClass()
    : m_position_x(0.0f),
    m_position_y(0.0f),
    m_position_z(0.0f),
    m_rotation_x(0.0f),
    m_rotation_y(0.0f),
    m_rotation_z(0.0f)
{
}

void CameraClass::set_position(float x, float y, float z)
{
    m_position_x = x;
    m_position_y = y;
    m_position_z = z;
}

void CameraClass::set_rotation(float x, float y, float z)
{
    m_rotation_x = x;
    m_rotation_y = y;
    m_rotation_z = z;
}

XMFLOAT3 CameraClass::get_position() const
{
    return XMFLOAT3(m_position_x, m_position_y, m_position_z);
}

XMFLOAT3 CameraClass::get_rotation() const
{
    return XMFLOAT3(m_rotation_x, m_rotation_y, m_rotation_z);
}

void CameraClass::render()
{
    XMFLOAT3 up, pos, lookat;
    XMVECTOR vec_up, vec_pos, vec_lookat;
    float yaw, pitch, roll;
    XMMATRIX mat_rot;

    constexpr float rad = 0.0174532925f;

    up.x = 0.0f;
    up.y = 1.0f;
    up.z = 0.0f;
    vec_up = XMLoadFloat3(&up);

    pos.x = m_position_x;
    pos.y = m_position_y;
    pos.z = m_position_z;
    vec_pos = XMLoadFloat3(&pos);

    lookat.x = 0.0f;
    lookat.y = 0.0f;
    lookat.z = 1.0f;
    vec_lookat = XMLoadFloat3(&lookat);

    pitch = m_rotation_x * rad;
    yaw = m_rotation_y * rad;
    roll = m_rotation_z * rad;

    mat_rot = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);
    
    // Transform lookat and up vectors by rotation matrix
    vec_lookat = XMVector3TransformCoord(vec_lookat, mat_rot);
    vec_up = XMVector3TransformCoord(vec_up, mat_rot);

    // Translate the camera
    vec_lookat = XMVectorAdd(vec_pos, vec_lookat);

    // Create the view matrix from transformed vectors
    m_mat_view = XMMatrixLookAtLH(vec_pos, vec_lookat, vec_up);
}

void CameraClass::get_view_matrix(DirectX::XMMATRIX &mat_view)
{
    mat_view = m_mat_view;
}