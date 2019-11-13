#pragma once
#include "stdafx.h"



LPTSTR _tCharAlloc(UINT uSize);
VOID _tCharFree(LPVOID p);
#define IMAGEAPI WINAPI

BOOL IMAGEAPI MakeSureDirectoryPathExists(LPCTSTR pszDirPath);