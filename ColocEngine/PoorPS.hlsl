#include "PoorHeader.hlsli"
//-----------

SamplerState colsmp : register(s0);
Texture2D	 colmap : register(t0);

PSoutput main(VSoutput inp)
{
	PSoutput resultÅ@= (PSoutput)0;
	float2 center = (inp.uv -0.5 )*2;

	result.color = colmap.Sample(colsmp, inp.uv);

	//result.color = smoothstep(0.2,0.4,sin(length(center) * 3.14 * 50));
	//result.color *= step(0.5, length(center));

	//result.color = 1 - length(center *2);
	//result.color = smoothstep(0.2, 0.7, result.color);

	/*
	float sub = exp((abs(center.x) *- 5)) *0.4;
	center.y -= sub ;
	center.y += 0.25;
	center.x *= 0.85;
	result.color = step(0.5, length(center)) * (1 - step(0.7, length(center)));
	result.color.r = 1 - step(0.7, length(center));
	*/
	/*
	center.x = clamp(-1, 0.0, center.x);
	result.color = step(0.2,(0.5,1 - length(center)) * length(center));
	result.color = result.color - step(0.5,(center.x));
	*/

	/*
	float2 tri = 0;
	tri.y = sin(center.y * 3.14 * 10 + pow(center.x *5,2) + center.x) + center.x ;
	
	result.color = length(tri.y);
	*/

	result.color =step(0.5,abs( sin(length(center)*3.14 *10)));
	float4 temp = result.color;

	result.color = 0.5;



	return result;
}