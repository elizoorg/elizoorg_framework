
cbuffer cbPerObject
{
	float4x4 gWorldViewProj;
    float4 offset;
    float4 scale;
    float4x4 rotation;
};

struct VS_IN
{
	float4 pos : POSITION0;
	float4 col : COLOR0;
};

struct PS_IN
{
	float4 pos : SV_POSITION;
	float4 col : COLOR;
};

PS_IN VSMain( VS_IN input )
{
	PS_IN output = (PS_IN)0;
	
    output.pos = float4(input.pos.xyz * scale.xyz, 1.0);
    output.pos = mul(rotation, output.pos);
    output.pos = mul(gWorldViewProj, output.pos);
    output.pos = output.pos + offset;
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