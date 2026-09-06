#pragma once
#include "d3dclass.h"
#include "cameraclass.h"
#include "modelclass.h"
#include "colorshaderclass.h"

// -- Globals --
constexpr bool FULL_SCREEN = false;
constexpr bool VSYNC_ENABLED = true;
constexpr float SCREEN_DEPTH = 1000.0f;
constexpr float SCREEN_NEAR = 0.3f;

class ApplicationClass
{
private:
    D3DClass *m_direct3d;
    CameraClass *m_cam;
    ModelClass *m_model;
    ColorShaderClass *m_color_shader;

    void update();
    bool render();
    
public:
    ApplicationClass();
    ApplicationClass(const ApplicationClass&) = default;
    ~ApplicationClass() = default;

    bool initialize(int, int, HWND);
    void shutdown();
    bool frame();
};