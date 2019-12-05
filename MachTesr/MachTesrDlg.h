
// MachTesrDlg.h: 头文件
//
#include "CanComm.h"

#include "SaveTestDate.h"
#include "ListCtrlCL.h"
#include "CAnalysisIni.h"
#include "CParsing.h"

#pragma once
#define MAX_LENGTH 24
#define ID_START	3
#define DATA_START 14
#define COUNT		5
#define TIME_OUT	1

// CMachTesrDlg 对话框
class CMachTesrDlg : public CDialogEx
{
// 构造
public:
	CMachTesrDlg(CWnd* pParent = nullptr);	// 标准构造函数
	~CMachTesrDlg();
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MACHTESR_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	CCanComm m_CanComm;
	CAnalysisIni m_AnalysisIni;
	SaveTestDate m_SaveTestDate;
	CParsing m_Parsing;

	bool InitDoneFlag = false;
	bool TestRunFlag = false;
	bool TimeOutFlag = true;
	bool ChangeFlag = false;

	bool CarLineFlag = false;
	bool CarWarnFlag = false;
	bool ObjNumbFlag = false;

	bool PassFlag = true;

	CString SaveData;
	CString SaveDataName;
	CString SaveDataHand;

	FrameData *m_FrameData = NULL;

public:
	INT m_Count = 0;
	CString c_Num;
	CString StrZunq;
	CString StrTest;
	CString StrId;
	CString StrData;
	CString StrNum[2] = {0};
	CString StrNum1;
	CString Path;
// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	void Init();
	void RemoveLineData();
	void ComparedFrame();
	void ResourceReset();
	static unsigned int TestThread(void* param);
public:

	CListCtrlCl m_List;
	CEdit m_Zynq;
	CEdit m_Num;
	CStatic m_Display;

	afx_msg void OnBnClickedInit();
	afx_msg void OnBnClickedChangeZynq();
	afx_msg void OnRclickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedTest();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnChangeEditNum();
	
	CButton m_Check1;
	CButton m_Check2;
	CButton m_Check3;
	CButton m_Check4;
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedCheck2();
	afx_msg void OnBnClickedCheck3();
	afx_msg void OnBnClickedCheck4();

	CString GetWorkDir();

	afx_msg void OnOK();

	afx_msg void OnCbnSelchangeCombo1();
	CComboBox m_cbCom;
	CString strCBText;
	UINT nCANIndex;
};
