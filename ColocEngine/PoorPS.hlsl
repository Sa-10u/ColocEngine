#include "PoorHeader.hlsli"
//-----------

SamplerState colsmp : register(s0);
Texture2D	 colmap : register(t0);

PSoutput main(VSoutput inp)
{
	PSoutput result@= (PSoutput)0;

	result.color = colmap.Sample(colsmp, inp.uv);

	return result;
}