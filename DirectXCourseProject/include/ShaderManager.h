#pragma once
#include "pch.h"
#include "Exports.h"


enum ShaderType
{
	Vertex = 1 << 0,
	Pixel = 1 << 1,
	Geometry = 1 << 2,
	Compute = 1 << 3
};
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-dynamic-exception-spec"
#endif

DEFINE_ENUM_FLAG_OPERATORS(ShaderType)


struct ShaderData {
  ShaderData(std::string _name, ShaderType _type) : name(_name),type(_type){
  
  };
	ShaderData(){};
	std::string name;
	ShaderType type;
};

namespace Engine
{
	class Application;
}



struct Shaders {
	ID3D11VertexShader* VShader = nullptr;
	ID3D11PixelShader* PShader = nullptr;
	ID3D11GeometryShader* GShader = nullptr;
	ID3D11ComputeShader* CShader = nullptr;
};

class ENGINE_API ShaderManager
{
public:
	ShaderManager();
	ShaderManager(Engine::Application* app) : _app(app)
	{



	};
	void InitShader(ShaderData data);
	void SetShader(ShaderData data);
	void CompileShaderFromFile(std::string shaderName, D3D_SHADER_MACRO shaderMacros[], LPCSTR entryPoint, LPCSTR shaderModel, ID3DBlob** blobout);
private:
	Engine::Application* _app;
	std::map<std::string, Shaders> shaders;

	ID3D11InputLayout* inputLayout = nullptr;

	HRESULT CreateInputLayoutDescFromVertexShaderSignature(ID3DBlob* pShaderBlob, ID3D11Device* pD3DDevice, ID3D11InputLayout** pInputLayout);

};

