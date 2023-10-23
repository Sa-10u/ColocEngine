#include "LoadFile.h"

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

	swprintf_s(dst, L"..\\%s",name);
	if (PathFileExistsW(dst))
	{
		*str = dst;
		return 1;
	}

	swprintf_s(dst, L"..\\..\\%s", name);
	if (PathFileExistsW(dst))
	{
		*str = dst;
		return 1;
	}

	swprintf_s(dst, L"\\res\\%s", name);
	if (PathFileExistsW(dst))
	{
		*str = dst;
		return 1;
	}

	swprintf_s(dst, L"%s\\%s",path ,name);
	if (PathFileExistsW(dst))
	{
		*str = dst;
		return 1;
	}

	swprintf_s(dst, L"%s\\..\\%s", path, name);
	if (PathFileExistsW(dst))
	{
		*str = dst;
		return 1;
	}

	swprintf_s(dst, L"%s\\..\\..\\%s", path, name);
	if (PathFileExistsW(dst))
	{
		*str = dst;
		return 1;
	}

	swprintf_s(dst, L"%s\\res\\%s", path, name);
	if (PathFileExistsW(dst))
	{
		*str = dst;
		return 1;
	}

	return 0;
}