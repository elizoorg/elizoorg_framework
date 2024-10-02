#pragma once

#include <include/Exports.h>
#include <include/Application.h>
#include <include/Display.h>
#include <include/InputDevice.h>
#include <include/TriangleComponent.h>
#include <include/GameComponent.h>
#include <include/Camera.h>
#include <include/MathTypes.h>
#include <include/SphereComponent.h>
#include <include/DebugRenderSysImpl.h>
#include <include/Transform.h>
#include <include/ModelLoader.h>
#include <include/GBuffer.h>
#include <include/ShaderManager.h>
#include <cstdlib>
#include <ctime>
#include <math.h>

#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>


#include <assimp/Importer.hpp>
#include <assimp/mesh.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <include/LightComponent.h>


using namespace Engine;

class ENGINE_API Application;

class GameApplication: public Engine::Application
	{
	public:

		GameApplication();
		~GameApplication();


	
		DebugRenderSysImpl* system;
		static GameApplication* instance;
		int Run() override;
		void CreateBackBuffer() override;
		void DestroyResources() override;
		void Draw() override;
		void EndFrame() override;
		void Exit() override;
		void Initialize() override;
		void MessageHandler() override;
		void PrepareFrame() override;
		void PrepareRecources() override;
		void RestoreTargets() override;
		bool Update() override;
		void UpdateInternal() override;




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
		void ResetGame();

		float angle = 0;

		aiNode* node;
		Assimp::Importer importer;

		ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;


		Microsoft::WRL::ComPtr<ID3D11SamplerState> depthSamplerState_ = nullptr;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> defaultDepthState_;

		Transform transform[7];

		float rand_FloatRange(float a, float b)
		{
			return ((b - a) * ((float)rand() / RAND_MAX)) + a;
		}



		Vector4 tmp = Vector4(20.0f, 50.0f, 20.0f, 0.0f);
		CSM_CONSTANT_BUFFER cascadeData = {};

		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> quadDepthState_;
		Microsoft::WRL::ComPtr<ID3D11RasterizerState> rastState_;
		Microsoft::WRL::ComPtr<ID3D11BlendState> blendState_;

		GBuffer *gBuffer_;

		Transform lightTransform;
		LightComponent* volume;


		Transform playerTransform;
		SphereComponent* player;
		BoundingSphere PlayerCollision{};

		std::vector<std::pair<int, Vector3>> collected;


		std::vector<BoundingSphere> foodSpheres;

		const Vector3 offset = { 0,0,-25 };

		float gameSize = 10.0f;


		Quaternion savedRot;
	};
