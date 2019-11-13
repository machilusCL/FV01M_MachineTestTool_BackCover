#pragma once

#include "stdafx.h"
#include "Windows.h"
#include <locale.h>


/*����:����һ���ṹ����Key�����ֺ�����*/
#define Num 15
struct _KEYNAME
{
	TCHAR szKeyBuf[Num][MAX_PATH];
	int nKeyNum;
	TCHAR szBuffer[MAX_PATH];
	TCHAR szValue[20][MAX_PATH];
	_KEYNAME()
	{
		TCHAR szBuffer[MAX_PATH] = { 0 };
		TCHAR szValue[20][MAX_PATH] = { 0 };
		memset(szKeyBuf, '0x0', sizeof(szKeyBuf) / sizeof(TCHAR));
		nKeyNum = 0;
	}
};


class CAnalysisIni
{
public:
	CAnalysisIni();
	~CAnalysisIni();
	int GetKeyName(int nLen, TCHAR* szBuf); //��ȡ�����ļ�ĳ��Section�е�Keyֵ��Key���ܸ���
	int GetKeyValue(TCHAR szValue[][MAX_PATH], struct _KEYNAME KeyName, CString szSection, TCHAR* szIniPath); //��ȡKey��Ӧ��ֵ
	int GetSectionAllKey(CString filename,CString Section);//��ȡĳ��Section�е�����Keyֵ
	int SetFilePath();
	struct _KEYNAME gKeyName;


};

