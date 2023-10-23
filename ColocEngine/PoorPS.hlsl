#include "PoorHeader.hlsli"
//-----------

SamplerState colsmp : register(s0);
Texture2D	 colmap : register(t0);

PSoutput main(VSoutput inp)
{
	PSoutput result@= (PSoutput)0;

	result.color = colmap.Sample(colsmp, inp.uv);
    //result.color = float4(inp.uv.x, inp.uv.y, 0.0f, 1.0f);

	//result.color.r = smoothstep(0.6, 0.9,abs(sin(inp.uv.x *3.14*10)));
	//result.color.b = smoothstep(0.6, 0.9,abs(sin(inp.uv.y * 3.14 * 10)));

	return result;
}