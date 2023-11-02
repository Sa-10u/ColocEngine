struct VSinput
{
	float3 pos	: POSITION;
    float3 norm : NORMAL;
	float2 uv	: TEXCOORD;
    float3 tan  : TANGENT;
};

struct VSoutput
{
	float4 pos	: SV_POSITION;
	float2 uv	: TEXCOORD;
};

struct PSoutput
{
	float4 color : SV_TARGET0;
};

cbuffer Transform : register(b0)
{
	float4x4 World		: packoffset(c0);
	float4x4 View		: packoffset(c4);
	float4x4 Projection : packoffset(c8);
    float Time			: packoffset(c12);
};


