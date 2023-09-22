struct VSinput
{
	float3 pos : POSITION;
	float4 RGBA: COLOR;
};

struct VSoutput
{
	float4 pos : SV_POSITION;
	float4 RGBA: COLOR;
};

cbuffer Transform : register(b0)
{
	float4x4 World		: packoffset(c0);
	float4x4 View		: packoffset(c4);
	float4x4 Projection : packoffset(c8);
}

struct PSoutput
{
	float4 color : SV_TARGET0;

};
