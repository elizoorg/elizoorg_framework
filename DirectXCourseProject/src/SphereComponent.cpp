#include "SphereComponent.h"
#include "Application.h"


SphereComponent::~SphereComponent()
{
	DestroyResources();
	std::cout << "We're creating triangle\n";
}

void SphereComponent::DestroyResources()
{
	g_pConstantBuffer11->Release();
	ib->Release();
	vb->Release();
}

void SphereComponent::Reload()
{
}

bool SphereComponent::Initialize()
{
	float radius = 0.5f;
	int sliceCount = 20;
	int stackCount = 20;
	float phiStep = DirectX::XM_PI / stackCount;
	float thetaStep = 2.0f * DirectX::XM_PI / sliceCount;

	points.push_back(Vector4(0, radius,0, 1 ));
	points.push_back(Vector4(0, 1, 0, 1 ));
	for (int i = 1; i <= stackCount - 1; i++)
	{
		float phi = i * phiStep;
		for (int j = 0; j <= sliceCount; j++)
		{
			float theta = j * thetaStep;
			Vector3 p = Vector3(
				(radius * sinf(phi) * cosf(theta)),
				(radius * cosf(phi)),
				(radius * sinf(phi) * sinf(theta))
			);
			points.push_back(Vector4(p.x,p.y,p.z,1 ));
			points.push_back(Vector4(p.x,p.y,p.z,1 ));
		}
	}
	points.push_back(Vector4(0, -radius, 0, 1 ));
	points.push_back(Vector4(0, -radius, 0, 1 ));

	for (int i = 1; i <= sliceCount; i++)
	{
		indeces.push_back(0);
		indeces.push_back(i + 1);
		indeces.push_back(i);
	}
	int baseIndex = 1;
	int ringVertexCount = sliceCount + 1;
	for (int i = 0; i < stackCount - 2; i++)
	{
		for (int j = 0; j < sliceCount; j++)
		{
			indeces.push_back(baseIndex + i * ringVertexCount + j);
			indeces.push_back(baseIndex + i * ringVertexCount + j + 1);
			indeces.push_back(baseIndex + (i + 1) * ringVertexCount + j);

			indeces.push_back(baseIndex + (i + 1) * ringVertexCount + j);
			indeces.push_back(baseIndex + i * ringVertexCount + j + 1);
			indeces.push_back(baseIndex + (i + 1) * ringVertexCount + j + 1);
		}
	}
	int southPoleIndex = points.size() / 2 - 1;
	baseIndex = southPoleIndex - ringVertexCount;
	for (int i = 0; i < sliceCount; i++)
	{
		indeces.push_back(southPoleIndex);
		indeces.push_back(baseIndex + i);
		indeces.push_back(baseIndex + i + 1);
	}

	for (Vector4 p : points) {
		DirectX::XMFLOAT4 XMp(p.x, p.y, p.z, p.w);
		dpoints.push_back(XMp);
	}



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
			MessageBox(_app->getDisplay()->getHWND(), L"MyVeryFirstShader.hlsl", L"Missing Shader File", MB_OK);
		}

		return false;
	}

	D3D_SHADER_MACRO Shader_Macros[] = { "1", "TCOLOR", "float4(0.0f, 1.0f, 0.0f, 1.0f)", nullptr, nullptr};

	res = D3DCompileFromFile(L"./Shaders/MyVeryFirstShader.hlsl", Shader_Macros /*macros*/, nullptr /*include*/,
	                         "PSMain", "ps_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION | D3DCOMPILE_PACK_MATRIX_COLUMN_MAJOR, 0, &pixelBC,
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
			MessageBox(_app->getDisplay()->getHWND(), L"MyVeryFirstShader.hlsl", L"Missing Shader File", MB_OK);
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
	vertexBufDesc.ByteWidth = sizeof(Vector4) * dpoints.size();

	D3D11_SUBRESOURCE_DATA vertexData = {};
	vertexData.pSysMem = dpoints.data();
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;


	_app->getDevice()->CreateBuffer(&vertexBufDesc, &vertexData, &vb);

	
	D3D11_BUFFER_DESC indexBufDesc = {};
	indexBufDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufDesc.CPUAccessFlags = 0;
	indexBufDesc.MiscFlags = 0;
	indexBufDesc.StructureByteStride = 0;
	indexBufDesc.ByteWidth = sizeof(int) * indeces.size();

	D3D11_SUBRESOURCE_DATA indexData = {};
	indexData.pSysMem = indeces.data();
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	//ID3D11Buffer* ib;
	_app->getDevice()->CreateBuffer(&indexBufDesc, &indexData, &ib);


	CD3D11_RASTERIZER_DESC rastDesc = {};
	rastDesc.CullMode = D3D11_CULL_NONE;
	rastDesc.FillMode = D3D11_FILL_SOLID;
	rastDesc.FrontCounterClockwise = true;

	//ID3D11RasterizerState* rastState;
	res = _app->getDevice()->CreateRasterizerState(&rastDesc, &rastState);


}

void SphereComponent::Update(Matrix cameraProjection, Matrix cameraView, Matrix world)
{
	if (g_pConstantBuffer11) {
		g_pConstantBuffer11->Release();
	}
	buffer.cameraProj = cameraProjection;
	buffer.cameraView = cameraView;
	buffer.world = world;

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



void SphereComponent::Update()
{

}

void SphereComponent::Draw()
{
	UINT strides[] = {sizeof(DirectX::XMFLOAT4)*2};
	UINT offsets[] = {0};
	_app->getContext()->RSSetState(rastState);
	_app->getContext()->OMSetDepthStencilState(_app->getStencilState().Get(), 0);
	_app->getContext()->IASetInputLayout(layout);
	_app->getContext()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	_app->getContext()->IASetIndexBuffer(ib, DXGI_FORMAT_R32_UINT, 0);
	_app->getContext()->IASetVertexBuffers(0, 1, &vb, strides, offsets);
	_app->getContext()->VSSetShader(vertexShader, nullptr, 0);
	_app->getContext()->PSSetShader(pixelShader, nullptr, 0);
	_app->getContext()->VSSetConstantBuffers(0, 1, &g_pConstantBuffer11);
	_app->getContext()->DrawIndexed(indeces.size(), 0, 0);
}
