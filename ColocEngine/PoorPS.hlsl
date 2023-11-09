#include "PoorHeader.hlsli"
#include "Functions.hlsli"
//-----------

SamplerState colsmp : register(s0);
Texture2D	 colmap : register(t0);

PSoutput main(VSoutput inp)
{
	float2 uv = (inp.uv -0.5) *2;

	float2 pos = (cuv(0.0, 0.0));
	float len = length(pos - inp.uv);

	float3 res =exp(1 - (len ) * 50);

	PSoutput o;
	o.color = float4(res,0);

	return o;
}