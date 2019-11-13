#include "stdafx.h"
#include "CParsing.h"
#include "MachTesrDlg.h"

CParsing::CParsing()
{
	
}


CParsing::~CParsing()
{
}



CString CParsing::ParsingCString(CString str)
{
	/*字符串中每项数据对齐，添加0x*/
	CString m_str;
	CString ctsr = str;
	CString temp;

	while (ctsr.GetLength() > 0)
	{

		int n = ctsr.Find(_T(" "));//查找第一个" "的位置
		if (n < 0)
		{
			if (ctsr.GetLength() <= 2)
			{
				if (ctsr.GetLength() == 1)
				{
					temp = _T("0x0") + ctsr;
					m_str = m_str + temp;
				}
				else if (ctsr.GetLength() == 2)
				{
					temp = _T("0x") + ctsr;
					m_str = m_str + temp;
				}

				break;
			}
			else if (ctsr.GetLength() <= 4)
			{
				if (temp.Find(_T("x")) > 0)
				{
					if (ctsr.GetLength() == 4)
					{
						m_str = m_str + ctsr;
					}
					else if (ctsr.GetLength() == 3)
					{
						ctsr = ctsr.Right(1);
						m_str = m_str + +_T("0x0") + ctsr;
					}
				}
				else if (temp.Find(_T("X")) > 0)
				{
					if (ctsr.GetLength() == 4)
					{
						temp = ctsr.Right(2);
						temp = _T("0x") + temp;
						m_str = m_str + temp;
					}
					else if (ctsr.GetLength() == 3)
					{
						ctsr = ctsr.Right(1);
						m_str = m_str + +_T("0x0") + ctsr;
					}

				}
				break;
			}
			else if (ctsr.GetLength() > 4)
			{
				AfxMessageBox(_T("测试数据输入有误\n"));
				break;
			}
		}
		temp = ctsr.Left(n);            //将" "左边的值取出给temp
		int length = ctsr.Delete(0, n + 1); //找出" "右边的字符串位数
		ctsr = ctsr.Right(length);      //将" "右边的字符串取出做为新的字符串
		if (temp.Find(_T("x")) > 0)
		{
			if (temp.GetLength() == 4) {
				m_str = m_str + temp + _T(" ");
			}
			else if (temp.GetLength() == 3)
			{
				temp = temp.Right(1);
				m_str = m_str + _T("0x0") + temp + _T(" ");
			}

		}
		else if (temp.Find(_T("X")) > 0) {
			if (temp.GetLength() == 4) {
				temp = temp.Right(2);
				temp = _T("0x") + temp;
				m_str = m_str + temp + _T(" ");
			}
			else if (temp.GetLength() == 3) {
				temp = temp.Right(1);
				temp = _T("0x0") + temp;
				m_str = m_str + temp + _T(" ");
			}

		}
		else
		{
			if (temp.GetLength() == 2) {
				temp = _T("0x") + temp;
				m_str = m_str + temp + _T(" ");
			}
			else if (temp.GetLength() == 1) {
				temp = _T("0x0") + temp;
				m_str = m_str + temp + _T(" ");
				printf("--------------\n");
			}
		}
	}
	return m_str;
}

CString CParsing::ParsingCString1(CString str) {

	/*字符串中每项数据对齐，去除0x*/
	CString m_str;
	CString ctsr = str;
	CString temp;

	while (ctsr.GetLength() > 0)
	{

		int n = ctsr.Find(_T(" "));		//查找第一个" "的位置
		if (n < 0)
		{
			if (ctsr.GetLength() <= 2)
			{
				if (ctsr.GetLength() == 2)
				{
					temp = ctsr;
					m_str = m_str + temp;
				}
				else if (ctsr.GetLength() == 1) {
					temp = _T("0") + ctsr;
					m_str = m_str + temp;
				}
				break;
			}
			else if (ctsr.GetLength() <= 4)
			{
				if (temp.Find(_T("x")) > 0)
				{
					if (ctsr.GetLength() == 4)
					{
						m_str = m_str + ctsr.Right(2);
					}
					else if (ctsr.GetLength() == 3)
					{
						ctsr = ctsr.Right(1);
						m_str = m_str + +_T("0") + ctsr;
					}
				}
				else if (temp.Find(_T("X")) > 0)
				{
					if (ctsr.GetLength() == 4)
					{
						m_str = m_str + ctsr.Right(2);
					}
					else if (ctsr.GetLength() == 3)
					{
						ctsr = ctsr.Right(1);
						m_str = m_str + +_T("0") + ctsr;
					}
				}
				break;
			}
			else if (ctsr.GetLength() > 4)
			{
				AfxMessageBox(_T("测试数据输入有误\n"));
				break;
			}
		}
		temp = ctsr.Left(n);            //将" "左边的值取出给temp
		int length = ctsr.Delete(0, n + 1); //找出" "右边的字符串位数
		ctsr = ctsr.Right(length);      //将" "右边的字符串取出做为新的字符串
		if (temp.Find(_T("x")) > 0)
		{
			if (temp.GetLength() == 4) {
				temp = temp.Right(2);
				m_str = m_str + temp + _T(" ");
			}
			else if (temp.GetLength() == 3)
			{
				temp = temp.Right(1);
				m_str = m_str + _T("0") + temp + _T(" ");
			}

		}
		else if (temp.Find(_T("X")) > 0) {

			if (temp.GetLength() == 4) {
				temp = temp.Right(2);
				m_str = m_str + temp + _T(" ");
			}
			else if (temp.GetLength() == 3)
			{
				temp = temp.Right(1);
				m_str = m_str + _T("0") + temp + _T(" ");
			}
		}
		else
		{
			if (temp.GetLength() == 2) {
				m_str = m_str + temp + _T(" ");
			}
			else if (temp.GetLength() == 1) {
				m_str = m_str + _T("0") + temp + _T(" ");
			}

		}
	}
	return m_str;
}

bool CParsing::FillingFrameData(FrameData *mFrameData) {
	CMachTesrDlg *dlg = (CMachTesrDlg*)AfxGetApp()->GetMainWnd();	//获取主窗口指针
	if (mFrameData == NULL)
	{
		return false;
	}
	/*解析字符串,存入FrameData结构体*/
	for (int i = 0; i < dlg->m_Count; i++)
	{
		CString temp = dlg->m_List.GetItemText(i + 2, 1);
		INT length = temp.GetLength();
		CString Temp_ID = temp.Mid(ID_START, COUNT);
		CString Temp_Data = temp.Mid(DATA_START, length - DATA_START);
		INT x;
		::StrToIntEx(Temp_ID.GetString(), STIF_SUPPORT_HEX, &x);
		mFrameData[i].ID = x;
		printf("ID:0x%x \n", mFrameData[i].ID);
		printf("Data:");
		Temp_Data = ParsingCString(Temp_Data); //统一格式为0x**;
		INT len = Temp_Data.GetLength();

		INT k = 0;
		for (INT j = 0; j < len; j += COUNT)
		{
			CString _temp = Temp_Data.Mid(j, COUNT);
			::StrToIntEx(_temp.GetString(), STIF_SUPPORT_HEX, &x);
			mFrameData[i].Data[k] = x;
			printf("0x%x ", mFrameData[i].Data[k]);
			k++;
		}
		mFrameData[i].DataLen = k;
		printf("len:%d\n", k);
		printf("\n");
	}
	return true;
}

bool CParsing::FillingFrameData1(FrameData *mFrameData) {
	CMachTesrDlg *dlg = (CMachTesrDlg*)AfxGetApp()->GetMainWnd();	//获取主窗口指针
	if (mFrameData == NULL)
	{
		return false;
	}
	/*解析字符串,存入FrameData结构体*/
	for (int i = 0; i < dlg->m_Count; i++)
	{
		CString temp = dlg->m_List.GetItemText(i + 2, 1);
		INT length = temp.GetLength();
		CString Temp_ID = _T("0x") + temp.Mid(ID_START, COUNT);
		INT x;
		::StrToIntEx(Temp_ID.GetString(), STIF_SUPPORT_HEX, &x);
		mFrameData[i].ID = x;
		printf("ID:0x%x \n", mFrameData[i].ID);
		switch (mFrameData[i].ID) {
		case 0x623:
			mFrameData[i].DataLen = 2;
			break;
		case 0x621:
			mFrameData[i].DataLen = 1;
			break;
		case 0x620:
			mFrameData[i].DataLen = 1;
			break;
		case 0x624:
			mFrameData[i].DataLen = 1;
			break;
		default:
			break;
		}
		printf("len:%d\n", mFrameData[i].DataLen);
	}

	return true;
}