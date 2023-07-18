#include "PoorHeader.hlsli"

//-----------

PSoutput main(VSoutput inp)
{
	PSoutput result;

	result.color = inp.RGBA;

	return result;
}