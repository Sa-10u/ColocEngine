struct VSinput
{
	float3 pos : POS;
	float4 RGBA: COLOR;
};

struct VSoutput
{
	float4 pos : SV_POSITION;
	float4 RGBA: COLOR;
};

cbuffer Transform : register(b0)
{
	float4x4 

}