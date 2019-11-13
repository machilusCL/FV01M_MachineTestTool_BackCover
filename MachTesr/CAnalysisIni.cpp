#include "stdafx.h"
#include "CAnalysisIni.h"

TCHAR strFilePath[MAX_PATH] = { 0 };

CAnalysisIni::CAnalysisIni()
{

}

CAnalysisIni::~CAnalysisIni()
{
}

int CAnalysisIni::GetKeyName(int nLen, TCHAR* szBuf) {

	int nIndex=0, nNum=0;

	if (nLen == 0 || sizeof(szBuf) == 0)
		return 0;

	for (int nPos = 0; nPos < nLen; nPos++)
	{
		if (szBuf[nPos] != _T('\0'))
		{
			gKeyName.szKeyBuf[nNum][nIndex++] = szBuf[nPos];
		}
		else
		{
			gKeyName.szKeyBuf[nNum][nIndex] = _T('\0');
			nNum++;
			nIndex = 0;
		}
	}

	gKeyName.nKeyNum = nNum;

	return nNum;

}

int CAnalysisIni::GetKeyValue(TCHAR szValue[][MAX_PATH], struct _KEYNAME KeyName, CString szSection, TCHAR* szIniPath)
{
	int nKeyNum = KeyName.nKeyNum;

	for (int i = 0; i < nKeyNum; i++)
	{
		GetPrivateProfileString(szSection, KeyName.szKeyBuf[i], _T(""), szValue[i],
			sizeof(szValue[i]) / sizeof(TCHAR), szIniPath);
	}

	return nKeyNum;
}

int CAnalysisIni::SetFilePath()
{
	TCHAR strExePath[MAX_PATH] = { 0 };
	GetModuleFileName(NULL, strExePath, MAX_PATH);

	TCHAR* pszPos = _tcsrchr(strExePath, _T('\\'));
	if (pszPos == NULL)
		return 0;

	*pszPos = 0x0;
	pszPos = strExePath;

	TCHAR* pszRelativePath = _T(".\\Config.ini");

	memset(strFilePath, 0x0, sizeof(strFilePath) / sizeof(TCHAR));
	swprintf_s(strFilePath, _T("%s%s"), pszPos, pszRelativePath);

	return 1;
}

int CAnalysisIni::GetSectionAllKey(CString filename,CString Section)
{
	int nKeyNum = 0;
	TCHAR* pszRelativePath = filename.GetBuffer();
	filename.ReleaseBuffer();
	swprintf_s(strFilePath, _T("%s"), pszRelativePath);
	/*获取Section下所有的Key的名字(*第二个参数为NULL*)*/
	unsigned long dwCharacterNum = GetPrivateProfileString(Section, NULL, _T(""), gKeyName.szBuffer,
		sizeof(gKeyName.szBuffer) / sizeof(TCHAR), strFilePath);

	/*分解出所有的Key值和Key的总个数*/
	nKeyNum = GetKeyName(dwCharacterNum, gKeyName.szBuffer);

	/*分别获取Key所对应的键值.保存到szValue中.*/
	int nRet = GetKeyValue(gKeyName.szValue, gKeyName, Section, strFilePath);


	return 0;
}