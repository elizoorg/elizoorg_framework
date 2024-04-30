#pragma once
#include "Exports.h"
#include "MathTypes.h"

namespace Engine
{
	class Application;
}

struct ENGINE_API VertexPositionTex
{
	VertexPositionTex() = default;
	VertexPositionTex(DirectX::SimpleMath::Vector4 position, DirectX::SimpleMath::Vector4 texture) : Position(position), Texture(texture)
	{}

	DirectX::SimpleMath::Vector4 Position;
	DirectX::SimpleMath::Vector4 Texture;

	static const UINT Stride = 32;

	static ID3D11InputLayout* GetLayout(ID3DBlob* signature, Engine::Application* app);

};
