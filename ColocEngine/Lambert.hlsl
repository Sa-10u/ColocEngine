#include"PoorHeader.hlsli"

SamplerState colsmp : register(s0);
Texture2D colmap : register(t0);

float4 main() : SV_TARGET
{
	return float4(1.0f, 1.0f, 1.0f, 1.0f);
}