struct VSoutput
{
	float4 pos : SV_POSITION;
	float4 RGBA: COLOR;
};

struct PSoutput
{
	float4 color : SV_TARGET0;

};
//-----------

PSoutput main(VSoutput inp)
{
	PSoutput result;

	result.color = inp.RGBA;

	return result;
}