#pragma once
#ifndef SCENE_H
#define SCENE_H
#include "Core.h"
#include "OrbitCamera.h"
#include "FirstPersonCamera.h"
#include <string>

struct SceneInitData
{
    ID3D11Device* device;
    ID3D11DeviceContext* deviceCtx;
    IDXGISwapChain* swapChain;
    ID3D11Buffer* constantBuffer;

    SceneInitData() {}
    SceneInitData(ID3D11Device* _device, ID3D11DeviceContext* _deviceCtx, IDXGISwapChain* _swapChain, ID3D11Buffer* _constantBuffer)
    {
        device         = _device;
        deviceCtx      = _deviceCtx;
        swapChain      = _swapChain;
        constantBuffer = _constantBuffer;
    }
};

class Scene
{
public:
	Scene(std::string name) : m_SceneName(name)
    {
        m_StaticCam   = nullptr;
        m_OrbitCamera = nullptr;
        m_FpsCam      = nullptr;
    }

    virtual void Init(SceneInitData data)   = 0;
    virtual void Enter()                    = 0;
    virtual void Update()                   = 0;
    virtual void Render(ConstantBuffer* cb) = 0;
    virtual void Exit()                     = 0;
    virtual CameraMats GetActiveCamera()    = 0;
    

public:
    ID3D11Device* m_Device;
    ID3D11DeviceContext* m_DeviceContext;
    IDXGISwapChain*      m_SwapChain;
    ID3D11Buffer*        m_ContantBuffer;
    std::string          m_SceneName = "";
    bool                 m_Initialised = false;

    Camera*            m_StaticCam;
    FirstPersonCamera* m_FpsCam;
    OrbitCamera*       m_OrbitCamera;
    CameraMats         m_CurrentCamera;
};
#endif // !SCENE_H