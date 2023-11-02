#include "PoorHeader.hlsli"

///----------

VSoutput main(VSinput inp)
{
	VSoutput result = (VSoutput)0;

	float4 posLoc = float4(inp.pos, 1.0f);
	float4 posWld = mul(World, posLoc);
	float4 posView = mul(View, posWld);
	float4 posProj = mul(Projection, posView);

	result.pos = posProj;
	//result.pos = float4(inp.pos,1.0f);

	result.uv = inp.uv;

	return result;
}