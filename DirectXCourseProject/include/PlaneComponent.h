#pragma once
#include "Exports.h"
#include "GameComponent.h"
#include "SimpleMath.h"
#include "MathTypes.h"

class ENGINE_API PlaneComponent :
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
    struct Point
    {
        DirectX::SimpleMath::Vector4 pos;
        DirectX::SimpleMath::Vector4 col;
    };

    struct VS_CONSTANT_BUFFER
    {
        Matrix world;
        Matrix cameraView;
        Matrix cameraProj;
    } buffer;


    // TODO: It doesnt work like that , points must declared somwhere else
    std::vector<Point> points;
    std::vector<int> indices;

    ID3DBlob* pixelB = nullptr;
    ID3DBlob* errorPixelCode = nullptr;

    ID3DBlob* vertexBC = nullptr;
    ID3DBlob* errorVertexCode = nullptr;


    ID3D11Buffer* ib;
    ID3DBlob* pixelBC;



    ID3D11Buffer* vb;
    HRESULT res;

public:
    PlaneComponent(Engine::Application* app) : GameComponent(app) {
        float cellSize = 1.0f;
        int lineCount = 100;
        constexpr Vector4 color = Vector4(0.15f, 0.15f, 0.15f, 1.0f);
        constexpr Vector4 boldColor = Vector4(0.3f, 0.3f, 0.3f, 1.0f);

        int k = 0;
        for (int i = 0 - lineCount / 2; i < lineCount / 2; ++i)
        {
            points.push_back(Point({ Vector4(static_cast<float>(i), 0.0f, -cellSize * lineCount / 2, 1.0f), i % 5 == 0 ? boldColor : color }));
            indices.push_back(k++);
            points.push_back(Point({ Vector4(static_cast<float>(i), 0.0f, cellSize * lineCount / 2, 1.0f), i % 5 == 0 ? boldColor : color }));
            indices.push_back(k++);
        }
        for (int i = 0 - lineCount / 2; i < lineCount / 2; ++i)
        {
            points.push_back(Point({ Vector4(-cellSize * lineCount / 2, 0.0f, static_cast<float>(i), 1.0f), i % 5 == 0 ? boldColor : color }));
            indices.push_back(k++);
            points.push_back(Point({ Vector4(cellSize * lineCount / 2, 0.0f, static_cast<float>(i), 1.0f), i % 5 == 0 ? boldColor : color }));
            indices.push_back(k++);
        }
    };
    ~PlaneComponent();
    void DestroyResources();
    void Reload();
    bool Initialize();
    virtual void Update(Matrix cameraProjection, Matrix cameraView, Matrix world);
    void Update();
    void Draw();
};

