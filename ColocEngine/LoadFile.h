#pragma once
#include <string>
#include<Shlwapi.h>

#pragma comment(lib,"shlwapi.lib")

using std::wstring;

bool FilePath(LPCWSTR name, wstring* str);
