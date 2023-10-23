#include "PoorHeader.hlsli"
//-----------

SamplerState colsmp : register(s0);
Texture2D	 colmap : register(t0);

PSoutput main(VSoutput inp)
{
	PSoutput result@= (PSoutput)0;

	//result.color = colmap.Sample(colsmp, inp.uv);
    result.color = float4(inp.uv.x, inp.uv.y, 0.0f, 1.0f);

	//result.color.r = inp.uv.x;


	return result;
}