struct VSinput
{
	float3 pos : POSITION;
	float4 RGBA: COLOR;
};

struct VSoutput
{
	float4 pos : SV_POSITION;
	float4 RGBA: COLOR;
};

cbuffer Transform : register(b0)
{
	float4x4 World		: packoffset(c0);
	float4x4 View		: packoffset(c4);
	float4x4 Projection : packoffset(c8);
}

///----------

VSoutput main(VSinput inp)
{
	VSoutput result = (VSoutput)0;

	float4 posLoc = float4(inp.pos, 1.0f);
	float4 posWld = mul(World, posLoc);
	float4 posView = mul(View, posWld);
	float4 posProj = mul(Projection, posView);

	result.pos = posProj;
	result.RGBA = inp.RGBA;

	return result;
}