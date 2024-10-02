#pragma pack_matrix(row_major)

#ifndef CASCADE_COUNT
#define CASCADE_COUNT 4
#endif

struct VS_IN
{
	float3 pos : POSITION0;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL0;
};

struct PS_IN
{
	float4 pos : SV_POSITION;
 	float2 tex : TEXCOORD;
};

cbuffer cbPerObject : register(b0)
{
    float4x4 world;
    float4x4 cameraView;
    float4x4 cameraProj;
    float4x4 InvWorldView;
};

cbuffer cbPerScene : register(b1)
{
	float4 gLightPos;
	float4 gLightColor;
	float4 ambientSpecularPowType;
	float4x4 gT;
	float4x4 gView;
};

cbuffer cbCascade : register(b2)
{
	float4x4 gViewProj[CASCADE_COUNT + 1];
	float4 gDistances;
};

Texture2D<float4> DiffuseTex : register(t0);
Texture2D<float3> WorldPositions : register(t1);
Texture2D<float3> Normals : register(t2);
Texture2DArray CascadeShadowMap : register(t3);
SamplerComparisonState DepthSampler : register(s0);

PS_IN VSMain(VS_IN input)
{
	PS_IN output = (PS_IN)0;
	
    output.pos = mul(float4(input.pos.xyz, 1.0f), world);
    output.pos = mul(float4(output.pos.xyz, 1.0f), cameraView);
    output.pos = mul(float4(output.pos.xyz, 1.0f), cameraProj);
	
	
	return output;
}

float ShadowCalculation(float4 posWorldSpace, float4 posViewSpace, float dotN)
{
	float depthValue = abs(posViewSpace.z);

	int layer = -1;
	for (int i = 0; i < CASCADE_COUNT; ++i)
	{
		if (depthValue < gDistances[i])
		{
			layer = i;
			break;
		}
	}
	if (layer == -1)
	{
		layer = CASCADE_COUNT;
	}

	float4 posLightSpace = mul(float4(posWorldSpace.xyz, 1.0), gViewProj[layer]);
	float3 projCoords = posLightSpace.xyz / posLightSpace.w;

	projCoords = (mul(float4(projCoords, 1.0f), gT)).xyz;
	float currentDepth = projCoords.z;

	if (currentDepth > 1.0f)
	{
		return 0.0f;
	}

	float bias = max(0.01f * (1.0f - dotN), 0.005f);
	const float biasModifier = 0.5f;
	if (layer == CASCADE_COUNT)
	{
		bias *= 1 / (1000.0 * biasModifier);
	}
	else
	{
		bias *= 1 / (gDistances[layer] * biasModifier);
	}

	// PCF
	float shadow = 0.0f;
	float2 texelSize = 1.0f / 2048.0f;
	for (int x = -1; x <= 1; ++x)
	{
		for (int y = -1; y <= 1; ++y)
		{
			shadow += CascadeShadowMap.SampleCmp(DepthSampler, float3(projCoords.xy + float2(x, y) * texelSize, layer), currentDepth - bias);
		}
	}
	shadow /= 9.0f;

	return shadow;
}

float4 PSMain(PS_IN input) : SV_Target
{
	input.tex = mul(float4(input.pos.xyz, 1.0f), gT).xy;
	input.pos = float4(input.tex * float2(2, -2) + float2(-1, 1), 0, 1);
	
	float3 norm = normalize(Normals.Load(int3(input.pos.xy, 0)));
	float4 worldPos = float4(WorldPositions.Load(int3(input.pos.xy, 0)).xyz, 1.0f);
	float4 viewPos = mul(worldPos, gView);
	float3 viewDir = normalize(-viewPos.xyz);
	float4 objColor = float4(DiffuseTex.Load(int3(input.pos.xy, 0)).xyz, 1.0f);

	float shadow = 0.0f;
	float attenuation = 1.0f;
	float3 lightDir = float3(0.0f, 0.0f, 0.0f);

	[branch]
	if (ambientSpecularPowType.w == 0)
	{
		lightDir = normalize(gLightPos.xyz);
		shadow = ShadowCalculation(worldPos, viewPos, dot(norm, gLightPos));
	}
	else
	{
		lightDir = gLightPos.xyz - viewPos.xyz;
		attenuation = 1.0f / (1.0f + length(lightDir) * length(lightDir));
		lightDir = normalize(lightDir);
	}
	
	float4 ambient = ambientSpecularPowType.x * float4(gLightColor.xyz, 1.0f) * attenuation;

	float diff = max(dot(norm, lightDir), 0.0f) * attenuation;
	float4 diffuse = diff * float4(gLightColor.xyz, 1.0f);
	
	float3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), ambientSpecularPowType.z) * attenuation;
	float4 specular = objColor.w * spec * float4(gLightColor.xyz, 1.0f);
	
	float4 result = (ambient + (1.0f - shadow) * (diffuse + specular)) * objColor;
	
	return float4(result.xyz, 1.0f);
}
