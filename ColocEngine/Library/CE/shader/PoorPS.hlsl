#include "PoorHeader.hlsli"
#include "Functions.hlsli"
//-----------

SamplerState colsmp : register(s0);
Texture2D	 colmap : register(t0);

PSoutput main(VSoutput inp)
{
	PSoutput resÅ@= (PSoutput)0;
    
	float2 cuv = (inp.uv -0.5 )*2;
    float2 uv = 2 * cuv;
    float2 quv = uv - floor(uv);
    float2 qcuv = (quv - 0.5) * 2;
   
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

    return res;
}