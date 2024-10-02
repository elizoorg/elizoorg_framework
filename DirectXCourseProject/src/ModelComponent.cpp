#include "ModelComponent.h"
#include "Application.h"


ModelComponent::~ModelComponent()
{
	DestroyResources();
}



void ModelComponent::LoadModel(std::string path)
{
	loader.Load(_app->getDisplay()->getHWND(), _app->getDevice().Get(), _app->getContext(), path);
}

void ModelComponent::DestroyResources()
{
	g_pConstantBuffer11->Release();
	g_pConstantBuffer12->Release();


}

void ModelComponent::Reload()
{
}

bool ModelComponent::Initialize()
{
	


	CD3D11_RASTERIZER_DESC rastDesc = {};
	rastDesc.AntialiasedLineEnable = false;
	rastDesc.CullMode = D3D11_CULL_BACK;
	rastDesc.DepthBias = 0;
	rastDesc.DepthBiasClamp = 0.0f;
	rastDesc.DepthClipEnable = true;
	rastDesc.FillMode = D3D11_FILL_SOLID;
	rastDesc.FrontCounterClockwise = true;
	rastDesc.MultisampleEnable = false;
	rastDesc.ScissorEnable = false;
	rastDesc.SlopeScaledDepthBias = 0.0f;


	//ID3D11RasterizerState* rastState;
	res = _app->getDevice()->CreateRasterizerState(&rastDesc, &rastState);
	

	D3D11_SAMPLER_DESC sampDesc;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	//sampDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	sampDesc.BorderColor[0] = 1.0f;
	sampDesc.BorderColor[1] = 0.0f;
	sampDesc.BorderColor[2] = 0.0f;
	sampDesc.BorderColor[3] = 1.0f;
	sampDesc.MaxLOD = static_cast<float>(INT_MAX);
	sampDesc.MipLODBias = 0.f;
	sampDesc.MaxAnisotropy = 1;



	//sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;

	res = _app->getDevice()->CreateSamplerState(&sampDesc, &TexSamplerState);
	if (FAILED(res)) {
		std::cout << "Something is going wrong with texture sampler";
	}
		
	




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

void ModelComponent::Update(Matrix cameraProjection, Matrix cameraView, Matrix world, Matrix InverseView)
{
	buffer.cameraProj = cameraProjection;
	buffer.cameraView = cameraView;
	buffer.world = world;
	buffer.InvWorldView = InverseView;
}


void ModelComponent::Update()
{

}

void ModelComponent::Draw()
{

	D3D11_VIEWPORT viewport = {};
	viewport.Width = _app->getDisplay()->getWidth();
	viewport.Height = _app->getDisplay()->getHeight();
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1.0f;

	_app->getContext()->RSSetViewports(1, &viewport);



	_app->getContext()->RSSetState(rastState);


	//_app->getContext()->OMSetDepthStencilState(_app->getStencilState().Get(), 0);
	_app->getShaderManager()->SetShader(ShaderData("./Shaders/GBuffer.hlsl",Vertex|Pixel));


	_app->getContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	_app->getContext()->PSSetSamplers(0, 1, &TexSamplerState);


	//auto csm = _app->getCSM();


	//_app->getContext()->PSSetShaderResources(1, 1, &csm);

	//_app->getContext()->PSSetConstantBuffers(1, 1, _app->getLightBuffer().GetAddressOf());
	//_app->getContext()->PSSetConstantBuffers(2, 1, _app->getCascadeBuffer().GetAddressOf());

	_app->getContext()->UpdateSubresource(g_pConstantBuffer11, 0, nullptr, &buffer, 0, 0);


	_app->getContext()->VSSetConstantBuffers(0, 1, &g_pConstantBuffer11);






	loader.Draw(_app->getContext());
}

void ModelComponent::PrepareFrame()
{
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
	_app->getContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	_app->getContext()->PSSetShader(nullptr, nullptr, 0);

	_app->getContext()->VSSetConstantBuffers(0, 1, &g_pConstantBuffer11);

	_app->getContext()->GSSetConstantBuffers(0, 1, _app->getCascadeBuffer().GetAddressOf());


	loader.Draw(_app->getContext());
}
