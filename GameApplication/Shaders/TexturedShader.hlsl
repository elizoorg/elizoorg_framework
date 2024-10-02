cbuffer cbPerObject
{
    float4x4 world;
    float4x4 cameraView;
    float4x4 cameraProj;
};

struct VS_IN
{
    float4 pos : POSITION0;
    float4 tex : TEXCOORD0;
};

struct PS_IN
{
    float4 pos : SV_POSITION;
    float2 tex : TEXCOORD0;
};

Texture2D DiffuseMap : register(t0);
SamplerState Sampler : register(s0);


PS_IN VSMain(VS_IN input)
{
    PS_IN output = (PS_IN) 0;
	
	
    output.pos = mul(input.pos, world);
    output.pos = mul(float4(output.pos.xyz, 1.0f), cameraView);
    output.pos = mul(float4(output.pos.xyz, 1.0f), cameraProj);
	
	
    output.tex = input.tex.xy;
	
    return output;
}

float4 PSMain(PS_IN input) : SV_Target
{
    float4 color = DiffuseMap.SampleLevel(Sampler, input.tex.xy, 0);
    
    
    

    return color;
}