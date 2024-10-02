#pragma once

#include <wrl.h>
#include <d3d11.h>
#include "Exports.h"

namespace Engine
{
    class Application;
}


class ENGINE_API GBuffer
{
protected:
    Engine::Application* _app;
public:
    Microsoft::WRL::ComPtr<ID3D11Texture2D> albedoBuffer_;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> albedoSrv_;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> albedoRtv_;
    Microsoft::WRL::ComPtr<ID3D11Texture2D> normalBuffer_;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> normalSrv_;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> normalRtv_;
    Microsoft::WRL::ComPtr<ID3D11Texture2D> positionBuffer_;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> positionSrv_;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> positionRtv_;

    explicit GBuffer(Engine::Application* app );
    void Initialize();
};
