#pragma once
#include "Exports.h"
#include "Display.h"
#include "InputDevice.h"
#include "DebugRenderSysImpl.h"
#include "TriangleComponent.h"
#include "GameComponent.h"
#include "Camera.h"
#include "MathTypes.h"
#include "DirectionalLight.h"
#include "ShaderManager.h"
#include <cstdlib>
#include <ctime>
namespace Engine{

	class ENGINE_API Application
	{
	public:
		static Application* instance;
		virtual int Run() = 0;
		virtual void DestroyResources() = 0;
		virtual void Draw() = 0;
		virtual void CreateBackBuffer() = 0;
		virtual void EndFrame() = 0;
		virtual void Exit() = 0;
		virtual void Initialize() = 0;
		virtual void MessageHandler() = 0;
		virtual void PrepareFrame() = 0;
		virtual void PrepareRecources() = 0;
		virtual void RestoreTargets() = 0;
		virtual bool Update() = 0;
		virtual void UpdateInternal() = 0;

		virtual void ResetGame() = 0;


		bool intersect(Vector2 min_a, Vector2 max_a, Vector2 min_b, Vector2 max_b)
		{
			return (min_a.x <= max_b.x) &&
				(max_a.x >= min_b.x) &&
				(min_a.y <= max_b.y) &&
				(max_a.y >= min_b.y);
		}
		bool isClosed = false;

		int player1_score = 0;
		int player2_score = 0;

		bool isMouseUsed = true;

		Transform transform[6];

		Microsoft::WRL::ComPtr<ID3D11Device> getDevice() { return device; };
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> getStencilState() { return depthStencilState.Get(); };

		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> getCSM() {
			return depthShadowSrv;
		}




		Microsoft::WRL::ComPtr<ID3D11Buffer> getLightBuffer() { return LightBuffer; }
		Microsoft::WRL::ComPtr<ID3D11Buffer> getCascadeBuffer() { return cascadeCBuffer_; }

		ID3D11DeviceContext* getContext() { return context; };
		Camera* getCamera(){return camera;}


		WinApi_Display* getDisplay() { return _display; };
		DirectionalLight* getLight() { return light; }
		InputDevice* getInput() { return Device; }


		ShaderManager* manager;
		ShaderManager* getShaderManager() { return manager; }

		std::chrono::time_point<std::chrono::steady_clock> PrevTime = std::chrono::steady_clock::now();
		float	deltaTime = 0;
	protected:

		std::vector<GameComponent*> Components;

		WinApi_Display* _display;
		DXGI_SWAP_CHAIN_DESC swapDesc;
		Microsoft::WRL::ComPtr<ID3D11Device> device;
		ID3D11DeviceContext* context;
		IDXGISwapChain* swapChain;

		ID3D11Texture2D* backTex;
		ID3D11RenderTargetView* rtv;

		ID3D11RasterizerState* rastState;
		CD3D11_RASTERIZER_DESC rastDesc = {};

		Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencilBuffer;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilState;


		Microsoft::WRL::ComPtr<ID3D11Texture2D> shadowTextureArray;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthShadowDsv;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> depthShadowSrv;

		struct CSM_CONSTANT_BUFFER
		{
			Matrix ViewProj[5];
			Vector4 Distance;
		};
		
		Microsoft::WRL::ComPtr<ID3D11Buffer> cascadeCBuffer_;
		Microsoft::WRL::ComPtr<ID3D11Buffer> LightBuffer;
		LightData lightData{};

		Camera *camera;
		InputDevice *Device;
		DirectionalLight *light;


		
		float totalTime = 0;
		unsigned int frameCount = 0;


	};
}