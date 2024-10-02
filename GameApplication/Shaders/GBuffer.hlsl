#pragma pack_matrix(row_major)

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
    float4 normal : NORMAL;
    float4 worldPos : WORLDPOS;
};

struct GBuffer
{
    float4 DiffuseSpec : SV_Target0;
    float3 WorldPos : SV_Target1;
    float3 Normal : SV_Target1;
};

cbuffer cbPerObject : register(b0)
{
    float4x4 world;
    float4x4 cameraView;
    float4x4 cameraProj;
    float4x4 InvWorldView;
};

Texture2D DiffuseMap : register(t0);
SamplerState Sampler : register(s0);

PS_IN VSMain(VS_IN input)
{
    PS_IN output = (PS_IN) 0;
	
    output.pos = mul(float4(input.pos.xyz, 1.0f), world);
    output.pos = mul(float4(output.pos.xyz, 1.0f), cameraView);
    output.pos = mul(float4(output.pos.xyz, 1.0f), cameraProj);
    
    output.tex = input.tex;
    output.normal = mul(float4(input.normal.xyz, 0.0f), InvWorldView);
    output.worldPos = mul(float4(input.pos.xyz, 1.0f), world);
	
    return output;
}

[earlydepthstencil]
GBuffer PSMain(PS_IN input) : SV_Target
{
    GBuffer result = (GBuffer) 0;

    float4 objColor = DiffuseMap.SampleLevel(Sampler, input.tex.xy, 0);
	
    result.DiffuseSpec.xyz = objColor.xyz;
    result.DiffuseSpec.w = 0.5f;
    result.WorldPos = input.worldPos.xyz;
    result.Normal = normalize(input.normal.xyz);
	
    return result;
}
