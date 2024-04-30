#pragma once
#include "Exports.h"
#include "MathTypes.h"

namespace Engine
{
	class Application;
}

struct ENGINE_API VertexPositionNormalBinormalTangentColorTex
{
	VertexPositionNormalBinormalTangentColorTex() = default;
	VertexPositionNormalBinormalTangentColorTex(Vector4 position, Vector4 normales, Vector4 binormales, Vector4 tagnents, Vector4 color, Vector4 tex):
		pos(position),normal(normales),binormal(binormales),color(color),tex(tex) {

	}

	Vector4 pos		;
	Vector4 normal	;
	Vector4 binormal;
	Vector4 tangent ;
	Vector4 color	;
	Vector4 tex		;

	static const UINT Stride = 96;

	static ID3D11InputLayout* GetLayout(ID3DBlob* signature, Engine::Application* app);


};
