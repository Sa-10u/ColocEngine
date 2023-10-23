#pragma once
#include <string>
#include<Shlwapi.h>

#pragma comment(lib,"shlwapi.lib")

using std::wstring;

bool FilePath(LPCWSTR name, wstring* str)
{
	if (!name)	return 0;
	if (!wcscmp(name, L" ") || !wcscmp(name, L""))	return 0;
	//--------------------------------------------------------

	const int wMaxPath = _MAX_PATH * 2;

	wchar_t path[wMaxPath] = {};
	GetModuleFileNameW(nullptr, path, wMaxPath);
	path[wMaxPath - 1] = 0;
	PathRemoveFileSpecW(path);

	wchar_t dst[wMaxPath] = {};

	//----------------------------

	wcscpy_s(dst, path);
	if (PathFileExistsW(dst))
	{
		*str = dst;
		return 1;
	}


}