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
#include <cstdlib>
#include <ctime>





#include <include/imgui.h>
#include <include/imgui_impl_win32.h>
#include <include/imgui_impl_dx11.h>

class ENGINE_API Engine::Application;
	class GameApplication: public Engine::Application
	{
	public:

		GameApplication();
		~GameApplication();



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

		ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;

		Transform transform[6];


	};
