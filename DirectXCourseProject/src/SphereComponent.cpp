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
	float radius = 1.0f;
	int sliceCount = 16;
	int stackCount = 16;
	float phiStep = DirectX::XM_PI / stackCount;
	float thetaStep = 2.0f * DirectX::XM_PI / sliceCount;

	VERTEX topPoint({
			0.0f,radius,0.0f,Vector2(0.0f,0.0f),0.0f,1.0f,0.0f
		});

	VERTEX bottomPoint({
		0.0f,-radius,0.0f,Vector2(0.0f,1.0f),0.0f,-1.0f,0.0f
	});

	points.push_back(topPoint);
	for (int i = 1; i <= stackCount - 1; i++)
	{
		float phi = i * phiStep;
		for (int j = 0; j <= sliceCount; j++)
		{
			float theta = j * thetaStep;
			VERTEX p;
			p.X = radius * sinf(phi) * cosf(theta);
			p.Y = radius * cosf(phi);
			p.Z = radius * sinf(phi) * sinf(theta);
			p.texcoord.x = theta / XM_2PI;
			p.texcoord.y = phi / XM_PI;
			Vector3 pos = Vector3(p.X, p.Y, p.Z);
			pos.Normalize();
			p.NX = pos.x;
			p.NY = pos.y;
			p.NZ = pos.z;
			points.push_back(p);
		}
	}

	points.push_back(bottomPoint);

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
	int southPoleIndex = points.size() - 1;


	baseIndex = southPoleIndex - ringVertexCount;

	for (int i = 0; i < sliceCount; i++)
	{
		indeces.push_back(southPoleIndex);
		indeces.push_back(baseIndex + i);
		indeces.push_back(baseIndex + i + 1);
	}





	D3D11_BUFFER_DESC vertexBufDesc = {};
	vertexBufDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufDesc.CPUAccessFlags = 0;
	vertexBufDesc.MiscFlags = 0;
	vertexBufDesc.StructureByteStride = 0;
	vertexBufDesc.ByteWidth = sizeof(VERTEX) * points.size();

	D3D11_SUBRESOURCE_DATA vertexData = {};
	vertexData.pSysMem = points.data();
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

	_app->getDevice()->CreateBuffer(&indexBufDesc, &indexData, &ib);


	CD3D11_RASTERIZER_DESC rastDesc = {};
	rastDesc.CullMode = D3D11_CULL_NONE;
	rastDesc.FillMode = D3D11_FILL_SOLID;
	rastDesc.FrontCounterClockwise = true;

	//ID3D11RasterizerState* rastState;
	res = _app->getDevice()->CreateRasterizerState(&rastDesc, &rastState);


	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.Usage = D3D11_USAGE_DEFAULT;
	cbDesc.CPUAccessFlags = 0;
	cbDesc.MiscFlags = 0;
	cbDesc.StructureByteStride = 0;
	cbDesc.ByteWidth = sizeof(VS_CONSTANT_BUFFER);

	_app->getDevice()->CreateBuffer(&cbDesc, NULL,
		&g_pConstantBuffer11);






	return true;


}

void SphereComponent::LoadTexture(std::wstring path)
{
	const HRESULT result = CreateWICTextureFromFile(_app->getDevice().Get(), 
		_app->getContext(),path.c_str(), nullptr, &defaultTexture.texture);
}

void SphereComponent::Update(Matrix cameraProjection, Matrix cameraView, Matrix world, Matrix InverseView)
{
	buffer.cameraProj = cameraProjection;
	buffer.cameraView = cameraView;
	buffer.world = world;
	buffer.InvWorldView = InverseView;
}



void SphereComponent::Update()
{

}

void SphereComponent::Draw()
{
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;

	_app->getShaderManager()->SetShader(ShaderData("./Shaders/GBuffer.hlsl", Vertex | Pixel));

	_app->getContext()->RSSetState(rastState);
	_app->getContext()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	if (defaultTexture.texture) {
		_app->getContext()->PSSetShaderResources(0, 1, &defaultTexture.texture);
	}
	_app->getContext()->IASetIndexBuffer(ib, DXGI_FORMAT_R32_UINT, 0);
	_app->getContext()->IASetVertexBuffers(0, 1, &vb, &stride, &offset);

	_app->getContext()->UpdateSubresource(g_pConstantBuffer11, 0, nullptr, &buffer, 0, 0);
	_app->getContext()->VSSetConstantBuffers(0, 1, &g_pConstantBuffer11);
	_app->getContext()->DrawIndexed(indeces.size(), 0, 0);
}

void SphereComponent::PrepareFrame()
{
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	_app->getContext()->RSSetState(rastState);

	D3D11_VIEWPORT viewport = {};
	viewport.Width = 2048.0f;
	viewport.Height = 2048.0f;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1.0f;

	_app->getContext()->RSSetViewports(1, &viewport);


	_app->getShaderManager()->SetShader(ShaderData("./Shaders/csm.hlsl", Vertex | Geometry));
	_app->getContext()->PSSetShader(nullptr, nullptr, 0);


	_app->getContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	_app->getContext()->IASetIndexBuffer(ib, DXGI_FORMAT_R32_UINT, 0);
	_app->getContext()->IASetVertexBuffers(0, 1, &vb, &stride, &offset);

	_app->getContext()->UpdateSubresource(g_pConstantBuffer11, 0, nullptr, &buffer, 0, 0);
	_app->getContext()->VSSetConstantBuffers(0, 1, &g_pConstantBuffer11);

	_app->getContext()->GSSetConstantBuffers(0, 1, _app->getCascadeBuffer().GetAddressOf());

	_app->getContext()->DrawIndexed(indeces.size(), 0, 0);

}






