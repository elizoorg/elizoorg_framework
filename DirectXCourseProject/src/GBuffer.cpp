#include "GBuffer.h"


#include "Application.h"

GBuffer::GBuffer(Engine::Application* app) : _app(app)
{
}

void GBuffer::Initialize()
{
    const int width = _app->getDisplay()->getWidth();
    const int height = _app->getDisplay()->getHeight();

    D3D11_TEXTURE2D_DESC baseBuffDesc = {};
    baseBuffDesc.Width = width;
    baseBuffDesc.Height = height;
    baseBuffDesc.ArraySize = 1;
    baseBuffDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
    baseBuffDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    baseBuffDesc.MipLevels = 1;
    baseBuffDesc.SampleDesc.Count = 1;
    baseBuffDesc.SampleDesc.Quality = 0;
    baseBuffDesc.Usage = D3D11_USAGE_DEFAULT;
    baseBuffDesc.CPUAccessFlags = 0;
    baseBuffDesc.MiscFlags = 0;

    D3D11_SHADER_RESOURCE_VIEW_DESC baseSrvDesc = {};
    baseSrvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    baseSrvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    baseSrvDesc.Texture2D.MipLevels = 1;
    baseSrvDesc.Texture2D.MostDetailedMip = 0;

    D3D11_RENDER_TARGET_VIEW_DESC baseRtvDesc = {};
    baseRtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    baseRtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    baseRtvDesc.Texture2D.MipSlice = 0;

    D3D11_TEXTURE2D_DESC albedoBuffDesc = baseBuffDesc;
    D3D11_SHADER_RESOURCE_VIEW_DESC albedoSrvDesc = baseSrvDesc;
    D3D11_RENDER_TARGET_VIEW_DESC albedoRtvDesc = baseRtvDesc;
    auto res = _app->getDevice()->CreateTexture2D(&albedoBuffDesc, nullptr, albedoBuffer_.GetAddressOf());
    res = _app->getDevice()->CreateShaderResourceView(albedoBuffer_.Get(), &albedoSrvDesc, albedoSrv_.GetAddressOf());
    res = _app->getDevice()->CreateRenderTargetView(albedoBuffer_.Get(), &albedoRtvDesc, albedoRtv_.GetAddressOf());

    D3D11_TEXTURE2D_DESC normalBuffDesc = baseBuffDesc;
    normalBuffDesc.Format = DXGI_FORMAT_R8G8B8A8_SNORM;
    D3D11_SHADER_RESOURCE_VIEW_DESC normalSrvDesc = baseSrvDesc;
    normalSrvDesc.Format = DXGI_FORMAT_R8G8B8A8_SNORM;
    D3D11_RENDER_TARGET_VIEW_DESC normalRtvDesc = baseRtvDesc;
    normalRtvDesc.Format = DXGI_FORMAT_R8G8B8A8_SNORM;
    res = _app->getDevice()->CreateTexture2D(&normalBuffDesc, nullptr, normalBuffer_.GetAddressOf());
    res = _app->getDevice()->CreateShaderResourceView(normalBuffer_.Get(), &normalSrvDesc, normalSrv_.GetAddressOf());
    res = _app->getDevice()->CreateRenderTargetView(normalBuffer_.Get(), &normalRtvDesc, normalRtv_.GetAddressOf());

    D3D11_TEXTURE2D_DESC positionBuffDesc = baseBuffDesc;
    positionBuffDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    D3D11_SHADER_RESOURCE_VIEW_DESC positionSrvDesc = baseSrvDesc;
    positionSrvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    D3D11_RENDER_TARGET_VIEW_DESC positionRtvDesc = baseRtvDesc;
    positionRtvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    res = _app->getDevice()->CreateTexture2D(&positionBuffDesc, nullptr, positionBuffer_.GetAddressOf());
    res = _app->getDevice()->CreateShaderResourceView(positionBuffer_.Get(), &positionSrvDesc, positionSrv_.GetAddressOf());
    res = _app->getDevice()->CreateRenderTargetView(positionBuffer_.Get(), &positionRtvDesc, positionRtv_.GetAddressOf());
}
