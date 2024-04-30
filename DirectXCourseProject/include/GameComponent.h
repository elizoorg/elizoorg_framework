#pragma once
#include "Exports.h"
#include "SimpleMath.h"
#include "Transform.h"
#include "MathTypes.h"

namespace Engine {
	class Application;
}

class ENGINE_API GameComponent
{
protected:
	friend class Engine::Application;
	Engine::Application* _app;
	Transform transform;

public:
	GameComponent(Engine::Application* app) : _app(app){

	};
	GameComponent& operator= (const GameComponent& other) {
		if (this == &other)
			return *this;
		transform = std::move(other.transform);
		return *this;
			
	}
	~GameComponent();
	virtual void DestroyResources() = 0;
	virtual void Draw() = 0;
	virtual bool Initialize() = 0;
	virtual void Reload() = 0;
	virtual void Update() = 0;
	virtual void Update(Matrix cameraProjection, Matrix cameraView, Matrix world) = 0;
};