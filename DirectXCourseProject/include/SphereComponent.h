#pragma once
#include "Exports.h"
#include "GameComponent.h"
#include "include/SimpleMath.h"
#include "MathTypes.h"

class ENGINE_API SphereComponent :
    public GameComponent
{
private:
    ID3D11InputLayout* layout;
    ID3D11VertexShader* vertexShader;
    ID3D11PixelShader* pixelShader;
    ID3D11RasterizerState* rastState;

    ID3D11Buffer* g_pConstantBuffer11 = NULL;
    D3D11_BUFFER_DESC cbDesc;
    D3D11_SUBRESOURCE_DATA InitData;


    struct VS_CONSTANT_BUFFER
    {
        Matrix gWorldViewProj;
        Vector4 offset;
        Vector4 scale;
        Matrix rotation;
    } buffer;


    // TODO: It doesnt work like that , points must declared somwhere else
    std::vector<Vector4> points;
    std::vector<Vector4> dpoints;
    std::vector<int> indeces;

    ID3DBlob* pixelB = nullptr;
    ID3DBlob* errorPixelCode = nullptr;

    ID3DBlob* vertexBC = nullptr;
    ID3DBlob* errorVertexCode = nullptr;


    ID3D11Buffer* ib;
    ID3DBlob* pixelBC;



    ID3D11Buffer* vb;
    HRESULT res;

public:
    SphereComponent(Engine::Application* app) : GameComponent(app) {

    };
    ~SphereComponent();
    void DestroyResources();
    void Reload();
    bool Initialize();
    virtual void Update(DirectX::SimpleMath::Matrix mat, Vector3 offset, Vector3 scale, Matrix rotation);
    void Update();
    void Draw();
};

