#include"PoorHeader.hlsli"

SamplerState colsmp : register(s0);
Texture2D colmap : register(t0);

PSoutput main(VSoutput inp)
{
    PSoutput res = (PSoutput)0;
    
    res.color = colmap.Sample(colsmp, inp.uv);
    return res;
}