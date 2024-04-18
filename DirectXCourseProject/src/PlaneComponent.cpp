#include "PlaneComponent.h"
#include "Application.h"


PlaneComponent::~PlaneComponent()
{
	std::cout << "We're creating triangle\n";
}

void PlaneComponent::DestroyResources()
{
}

void PlaneComponent::Reload()
{
}

bool PlaneComponent::Initialize()
{
	res = D3DCompileFromFile(L"./Shaders/MyVeryFirstShader.hlsl",
		nullptr /*macros*/,
		nullptr /*include*/,
		"VSMain",
		"vs_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&vertexBC,
		&errorVertexCode);

	if (FAILED(res))
	{
		// If the shader failed to compile it should have written something to the error message.
		if (errorVertexCode)
		{
			auto compileErrors = static_cast<char*>(errorVertexCode->GetBufferPointer());

			std::cout << compileErrors << std::endl;
		}
		// If there was  nothing in the error message then it simply could not find the shader file itself.
		else
		{
			MessageBox(_app->getDisplay()->getHWND(), "MyVeryFirstShader.hlsl", "Missing Shader File", MB_OK);
		}

		return false;
	}

	D3D_SHADER_MACRO Shader_Macros[] = { "1", "TCOLOR", "float4(0.0f, 1.0f, 0.0f, 1.0f)", nullptr, nullptr };

	res = D3DCompileFromFile(L"./Shaders/MyVeryFirstShader.hlsl", Shader_Macros /*macros*/, nullptr /*include*/,
		"PSMain", "ps_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &pixelBC,
		&errorPixelCode);

	if (FAILED(res))
	{
		// If the shader failed to compile it should have written something to the error message.
		if (errorPixelCode)
		{
			auto compileErrors = static_cast<char*>(errorVertexCode->GetBufferPointer());

			std::cout << compileErrors << std::endl;
		}
		// If there was  nothing in the error message then it simply could not find the shader file itself.
		else
		{
			MessageBox(_app->getDisplay()->getHWND(), "MyVeryFirstShader.hlsl", "Missing Shader File", MB_OK);
		}

		return false;
	}


	_app->getDevice()->CreateVertexShader(
		vertexBC->GetBufferPointer(),
		vertexBC->GetBufferSize(),
		nullptr, &vertexShader);

	_app->getDevice()->CreatePixelShader(
		pixelBC->GetBufferPointer(),
		pixelBC->GetBufferSize(),
		nullptr, &pixelShader);

	D3D11_INPUT_ELEMENT_DESC inputElements[] = {
		D3D11_INPUT_ELEMENT_DESC{
			"POSITION",
			0,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			0,
			0,
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		},
		D3D11_INPUT_ELEMENT_DESC{
			"COLOR",
			0,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			0,
			D3D11_APPEND_ALIGNED_ELEMENT,
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		}
	};


	_app->getDevice()->CreateInputLayout(
		inputElements,
		2,
		vertexBC->GetBufferPointer(),
		vertexBC->GetBufferSize(),
		&layout);

	D3D11_BUFFER_DESC vertexBufDesc = {};
	vertexBufDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufDesc.CPUAccessFlags = 0;
	vertexBufDesc.MiscFlags = 0;
	vertexBufDesc.StructureByteStride = 0;
	vertexBufDesc.ByteWidth = sizeof(DirectX::XMFLOAT4) * std::size(points);

	D3D11_SUBRESOURCE_DATA vertexData = {};
	vertexData.pSysMem = points;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;


	_app->getDevice()->CreateBuffer(&vertexBufDesc, &vertexData, &vb);

	int indeces[] = {
		0, 1, 2,    // side 1
		2, 1, 3,
		4, 0, 6,    // side 2
		6, 0, 2,
		7, 5, 6,    // side 3
		6, 5, 4,
		3, 1, 7,    // side 4
		7, 1, 5,
		4, 5, 0,    // side 5
		0, 5, 1,
		3, 7, 2,  // side 6
		2, 7, 6,
	};
	D3D11_BUFFER_DESC indexBufDesc = {};
	indexBufDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufDesc.CPUAccessFlags = 0;
	indexBufDesc.MiscFlags = 0;
	indexBufDesc.StructureByteStride = 0;
	indexBufDesc.ByteWidth = sizeof(int) * std::size(indeces);

	D3D11_SUBRESOURCE_DATA indexData = {};
	indexData.pSysMem = indeces;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	//ID3D11Buffer* ib;
	_app->getDevice()->CreateBuffer(&indexBufDesc, &indexData, &ib);


	CD3D11_RASTERIZER_DESC rastDesc = {};
	rastDesc.CullMode = D3D11_CULL_NONE;
	rastDesc.FillMode = D3D11_FILL_WIREFRAME;

	//ID3D11RasterizerState* rastState;
	res = _app->getDevice()->CreateRasterizerState(&rastDesc, &rastState);


}

void PlaneComponent::Update(DirectX::SimpleMath::Matrix mat, Vector3 offset, Vector3 scale, Matrix rotation)
{
	buffer.gWorldViewProj = mat;
	buffer.offset = Vector4(offset.x, offset.y, offset.z, 1.0f);
	buffer.scale = Vector4(scale.x, scale.y, scale.z, 1.0f);;
	buffer.rotation = rotation;

	cbDesc.ByteWidth = sizeof(VS_CONSTANT_BUFFER);
	cbDesc.Usage = D3D11_USAGE_DYNAMIC;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbDesc.MiscFlags = 0;
	cbDesc.StructureByteStride = 0;

	// Fill in the subresource data.

	InitData.pSysMem = &buffer;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;
	_app->getDevice()->CreateBuffer(&cbDesc, &InitData,
		&g_pConstantBuffer11);


}


void PlaneComponent::Update()
{

}

void PlaneComponent::Draw()
{
	UINT strides[] = { sizeof(Vector4) };
	UINT offsets[] = { 0 };
	_app->getContext()->RSSetState(rastState);
	_app->getContext()->IASetInputLayout(layout);
	_app->getContext()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	_app->getContext()->IASetIndexBuffer(ib, DXGI_FORMAT_R32_UINT, 0);
	_app->getContext()->IASetVertexBuffers(0, 1, &vb, strides, offsets);
	_app->getContext()->VSSetShader(vertexShader, nullptr, 0);
	_app->getContext()->PSSetShader(pixelShader, nullptr, 0);
	_app->getContext()->VSSetConstantBuffers(0, 1, &g_pConstantBuffer11);
	_app->getContext()->DrawIndexed(36, 0, 0);
}
