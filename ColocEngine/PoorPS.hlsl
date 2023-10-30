#include "PoorHeader.hlsli"
#include "Functions.hlsli"
//-----------

SamplerState colsmp : register(s0);
Texture2D	 colmap : register(t0);

PSoutput main(VSoutput inp)
{
	PSoutput res�@= (PSoutput)0;
    
	float2 cuv = (inp.uv -0.5 )*2;
    float2 uv = 2 * cuv;
    float2 quv = uv - floor(uv);
    float2 qcuv = (quv - 0.5) * 2;
   /*
    cuv *= 0.25*0.5;
    float3 col = 0;
    for (float i = 0; i < 5; i++){

        cuv *= 2;
        cuv = cuv - floor(cuv);
        cuv -= 0.5;
        //cuv *= 2;
    
        float dist = length(cuv) * exp(-length(uv*0.25));
    
        col = Pallet1(length((inp.uv - 0.5) * 2) + Time);
//--------------------------------
   
        dist = sin((dist - Time * 0.1) * 3.14 * 2);
        dist = abs(dist);
        dist = 0.05 / dist;
        col *= dist;
        
        res.color += float4(col, 1);
    }
    */

    
    float i1 = abs(sin(2*Time - 0.5));
    float i2 = abs(sin(2*Time + 0.5));
    float i3 = abs(sin(2*Time + 1.0));
    float s1 = pow(1 - length(uv.x),3);
    float s2 = pow(1 - length(uv.y), 3);

    res.color =  abs(float4(i1 * s1, i2 * s1, i3 * s1, 1));
    res.color += abs(float4(i1 * s2, i2 * s2, i3 * s2, 1));
    
	return res ;
}