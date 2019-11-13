#pragma once

#include "stdafx.h"
#include "Windows.h"
#include <locale.h>


/*功能:定义一个结构体存放Key的名字和数量*/
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
	int GetKeyName(int nLen, TCHAR* szBuf); //获取配置文件某个Section中的Key值和Key的总个数
	int GetKeyValue(TCHAR szValue[][MAX_PATH], struct _KEYNAME KeyName, CString szSection, TCHAR* szIniPath); //获取Key对应的值
	int GetSectionAllKey(CString filename,CString Section);//获取某个Section中的所有Key值
	int SetFilePath();
	struct _KEYNAME gKeyName;


};

