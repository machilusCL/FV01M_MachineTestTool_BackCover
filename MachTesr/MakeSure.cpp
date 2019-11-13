#include "MakeSure.h"
#include "stdafx.h"

LPTSTR _tCharAlloc(UINT uSize)
{
	return (LPTSTR)malloc(sizeof(TCHAR) * uSize);
}

VOID _tCharFree(LPVOID p)
{
	free(p);
}
#define IMAGEAPI WINAPI
BOOL IMAGEAPI MakeSureDirectoryPathExists(LPCTSTR pszDirPath)
{
	LPTSTR p, pszDirCopy = NULL;
	DWORD dwAttributes;
	// Make a copy of the string for editing.
	__try
	{
		pszDirCopy = (LPTSTR)_tCharAlloc(lstrlen(pszDirPath) + 1);
		if (pszDirCopy == NULL)
			return FALSE;
		lstrcpy(pszDirCopy, pszDirPath);
		p = pszDirCopy;
		//  If the second character in the path is "\", then this is a UNC
		//  path, and we should skip forward until we reach the 2nd \ in the path.
		if ((*p == TEXT('\\')) && (*(p + 1) == TEXT('\\')))
		{
			p++;            // Skip over the first \ in the name.
			p++;            // Skip over the second \ in the name.
			//  Skip until we hit the first "\" (\\Server\).
			while (*p && *p != TEXT('\\'))
			{
				p = CharNext(p);
			}
			// Advance over it.
			if (*p)
			{
				p++;
			}
			//  Skip until we hit the second "\" (\\Server\Share\).
			while (*p && *p != TEXT('\\'))
			{
				p = CharNext(p);
			}
			// Advance over it also.
			if (*p)
			{
				p++;
			}
		}
		else if (*(p + 1) == TEXT(':')) // Not a UNC.  See if it's <drive>:
		{
			p++;
			p++;
			// If it exists, skip over the root specifier
			if (*p && (*p == TEXT('\\')))
			{
				p++;
			}
		}
		while (*p)
		{
			if (*p == TEXT('\\'))
			{
				*p = TEXT('\0');
				dwAttributes = GetFileAttributes(pszDirCopy);
				// Nothing exists with this name.  Try to make the directory name and error if unable to.
				if (dwAttributes == 0xffffffff)
				{
					if (!CreateDirectory(pszDirCopy, NULL))
					{
						if (GetLastError() != ERROR_ALREADY_EXISTS)
						{
							_tCharFree(pszDirCopy);
							return FALSE;
						}
					}
				}
				else
				{
					if ((dwAttributes & FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY)
					{
						// Something exists with this name, but it's not a directory... Error
						_tCharFree(pszDirCopy);
						return FALSE;
					}
				}
				*p = TEXT('\\');
			}
			p = CharNext(p);
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		// SetLastError(GetExceptionCode());
		_tCharFree(pszDirCopy);
		return FALSE;
	}
	_tCharFree(pszDirCopy);
	return TRUE;
}