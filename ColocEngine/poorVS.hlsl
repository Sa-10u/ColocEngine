#include "PoorHeader.hlsli"

///----------

VSoutput main(VSinput inp)
{
	VSoutput result = (VSoutput)0;

	float4 posLoc = float4(inp.pos, 1.0);
	float4 posWld = mul(World, posLoc);
	float4 posView = mul(View, posWld);
	float4 posProj = mul(Projection, posView);

	result.pos = posProj;
	result.RGBA = inp.RGBA;

	return result;
}