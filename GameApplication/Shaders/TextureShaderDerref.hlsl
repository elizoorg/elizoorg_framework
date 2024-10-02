#pragma pack_matrix(row_major)


#ifndef CASCADE_COUNT
#define CASCADE_COUNT 4
#endif

cbuffer cbPerObject:register(b0)
{
    float4x4 world;
    float4x4 cameraView;
    float4x4 cameraProj;
    float4x4 InvWorldView;
};


cbuffer cbPerScene : register(b1)
{
    float4 lightPos;
    float4 lightColor;
    float4 ambientSpecularPowType;
    float4x4 gT;
    float4x4 gView;
};


cbuffer cbCascade : register(b2)
{
    float4x4 gViewProj[CASCADE_COUNT + 1];
    float4 gDistances;
};


struct PS_IN
{
    float4 pos : SV_POSITION;
    float2 tex : TEXCOORD;
};

Texture2D<float4> DiffuseTex : register(t0);
Texture2D<float3> WorldPositions : register(t1);
Texture2D<float3> Normals : register(t2);
Texture2DArray CascadeShadowMap : register(t3);
SamplerComparisonState DepthSampler : register(s0);



PS_IN VSMain(uint id : SV_VertexID)
{
    PS_IN output = (PS_IN) 0;
    
    output.tex = float2(id & 1, (id & 2) >> 1);
    output.pos = float4(output.tex * float2(2, -2) + float2(-1, 1), 0, 1);
	
    return output;

    
}

static const float4 vCascadeColorsMultiplier[8] =
{
    float4(1.5f, 0.0f, 0.0f, 1.0f),
    float4(0.0f, 1.5f, 0.0f, 1.0f),
    float4(0.0f, 0.0f, 5.5f, 1.0f),
    float4(1.5f, 0.0f, 5.5f, 1.0f),
    float4(1.5f, 1.5f, 0.0f, 1.0f),
    float4(1.0f, 1.0f, 1.0f, 1.0f),
    float4(0.0f, 1.0f, 5.5f, 1.0f),
    float4(0.5f, 3.5f, 0.75f, 1.0f)
};



static float4 cascadeColor;

float ShadowCalculation(float4 posWorldSpace, float4 posViewSpace, float dotN)
{
    float depthValue = abs(posViewSpace.z);

    int layer = -1;
    for (int i = 0; i < CASCADE_COUNT; ++i)
    {
        if (depthValue < gDistances[i])
        {
            cascadeColor = vCascadeColorsMultiplier[i];
            layer = i;
            break;
        }
    }
    if (layer == -1)
    {
        cascadeColor = vCascadeColorsMultiplier[CASCADE_COUNT];
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

    float bias = max(0.05f * (1.0f - dotN), 0.005f);
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
    
    if (projCoords.z > 1.0)
    {
        shadow = 0.0f;
    }

	//float shadow = CascadeShadowMap.SampleCmp(DepthSampler, float3(projCoords.xy, layer), currentDepth);

    return shadow;
}



float4 PSMain(PS_IN input) : SV_Target
{
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
        lightDir = normalize(lightPos.xyz);
        shadow = ShadowCalculation(worldPos, viewPos, dot(norm, lightPos));
    }
    else
    {
        lightDir = lightPos.xyz - viewPos.xyz;
        attenuation = 1.0f / (1.0f + length(lightDir) * length(lightDir));
        lightDir = normalize(lightDir);
    }
	
    float4 ambient = ambientSpecularPowType.x * float4(lightColor.xyz, 1.0f) * attenuation;

    float diff = max(dot(norm, lightDir), 0.0f) * attenuation;
    float4 diffuse = diff * float4(lightColor.xyz, 1.0f);
	
    float3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), ambientSpecularPowType.z) * attenuation;
    float4 specular = objColor.w * spec * float4(lightColor.xyz, 1.0f);
	
    float4 result = (ambient + (1.0f - shadow) * (diffuse + specular)) * objColor;
	
    return float4(result.xyz, 1.0f);
}
