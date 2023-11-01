#include"MACRO.h"

namespace SEMANTICS_STR
{
	LPCSTR POSITION = "POSITION";
	LPCSTR COLOR = "COLOR";
	LPCSTR TEXCOORD = "TEXCOORD";
	LPCSTR TANGENT = "TANGENT";
	LPCSTR NORMAL = "NORMAL";
}

namespace SHADER_FILENAME
{
	LPCWSTR PoorVS = L"PoorVS.cso";
	LPCWSTR PoorPS = L"PoorPS.cso";

	LPCWSTR LambertPS = L"Lambert.cso";
}

char* wtoc(const wchar_t* str)
{
	auto len = WideCharToMultiByte(CP_UTF8, 0, str, -1, nullptr, 0, nullptr, nullptr);
	auto buf = new char[len];

	WideCharToMultiByte(CP_UTF8, 0, str, -1, buf, len, nullptr, nullptr);

	return buf;
}
