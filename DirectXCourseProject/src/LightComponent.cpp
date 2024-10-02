#include "LightComponent.h"
#include "Application.h"
LightComponent::LightComponent(Engine::Application* _app) : SphereComponent(_app)
{

}

void LightComponent::Draw()
{
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;

	_app->getShaderManager()->SetShader(ShaderData("./Shaders/LightVolume.hlsl", Vertex | Pixel));

	_app->getContext()->RSSetState(rastState);
	_app->getContext()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	_app->getContext()->IASetIndexBuffer(ib, DXGI_FORMAT_R32_UINT, 0);



	_app->getContext()->IASetVertexBuffers(0, 1, &vb, &stride, &offset);

	_app->getContext()->UpdateSubresource(g_pConstantBuffer11, 0, nullptr, &buffer, 0, 0);
	_app->getContext()->VSSetConstantBuffers(0, 1, &g_pConstantBuffer11);
	_app->getContext()->DrawIndexed(indeces.size(), 0, 0);
}
