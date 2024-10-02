#pragma once
#include "Exports.h"
#include "GameComponent.h"
#include "MathTypes.h"
#include "Mesh.h"
#include "SimpleMath.h"
#include "TextureLoader.h"

class ENGINE_API SphereComponent : public GameComponent {
protected:
  ID3D11RasterizerState *rastState;

  ID3D11Buffer *g_pConstantBuffer11 = NULL;
  D3D11_BUFFER_DESC cbDesc;
  D3D11_SUBRESOURCE_DATA InitData;

  struct VS_CONSTANT_BUFFER {
    Matrix world;
    Matrix cameraView;
    Matrix cameraProj;
    Matrix InvWorldView;
  } buffer;

  // TODO: It doesnt work like that , points must declared somwhere else
  std::vector<VERTEX> points;
  std::vector<int> indeces;
  HRESULT res;
  Texture defaultTexture;

  ID3D11Buffer *ib;

  ID3D11Buffer *vb;

public:
  SphereComponent(Engine::Application *app)
      : GameComponent(app){

        };
  ~SphereComponent();
  void DestroyResources();
  void Reload();
  bool Initialize();
  void LoadTexture(std::wstring path);
  virtual void Update(Matrix cameraProjection, Matrix cameraView, Matrix world,
                      Matrix InverseView);
  void Update();
  void Draw();
  void PrepareFrame();
};
