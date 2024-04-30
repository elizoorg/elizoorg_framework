
cbuffer cbPerObject
{
    float4x4 world;
    float4x4 cameraView;
    float4x4 cameraProj;
};

struct VS_IN
{
	float4 pos : POSITION0;
	float4 col : COLOR0;
};

struct PS_IN
{
    float3 PositionWorldSpace : POS_WORLD;
    float3 PositionViewSpace  : POS_VIEW;
    float4 PositionClipSpace  : SV_POSITION;
	float4 col : COLOR;
};

PS_IN VSMain( VS_IN input )
{
	PS_IN output = (PS_IN)0;
	
    output.PositionWorldSpace = mul(input.pos, world).xyz;
    output.PositionViewSpace = mul(float4(output.PositionWorldSpace, 1.0f), cameraView).xyz;
    output.PositionClipSpace = mul(float4(output.PositionViewSpace, 1.0f), cameraProj);
	
	output.col = input.col;
	
	return output;
}

float4 PSMain( PS_IN input ) : SV_Target
{
    float4 col = input.col;
#ifdef TEST
	if (input.pos.x > 400) col = TCOLOR;
#endif
	return col;
}