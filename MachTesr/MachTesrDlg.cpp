
// MachTesrDlg.cpp: 实现文件
//
#include "stdafx.h"
#include "MachTesr.h"
#include "MachTesrDlg.h"
#include "afxdialogex.h"
#include <vector>
using std::vector;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMachTesrDlg 对话框



CMachTesrDlg::CMachTesrDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MACHTESR_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	Path = GetWorkDir() + _T(".\\Config\\Config.ini");

}

CMachTesrDlg::~CMachTesrDlg()
{
	free(m_FrameData);
}

void CMachTesrDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_List);
	DDX_Control(pDX, IDC_EDIT1, m_Zynq);
	DDX_Control(pDX, IDC_DISPLAY, m_Display);
	DDX_Control(pDX, IDC_EDIT_NUM, m_Num);
	DDX_Control(pDX, IDC_CHECK1, m_Check1);
	DDX_Control(pDX, IDC_CHECK2, m_Check2);
	DDX_Control(pDX, IDC_CHECK3, m_Check3);
	DDX_Control(pDX, IDC_CHECK4, m_Check4);
	DDX_Control(pDX, IDC_COMBO1, m_cbCom);
}

BEGIN_MESSAGE_MAP(CMachTesrDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_INIT, &CMachTesrDlg::OnBnClickedInit)
	ON_BN_CLICKED(IDC_CHANGE_ZYNQ, &CMachTesrDlg::OnBnClickedChangeZynq)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CMachTesrDlg::OnRclickList1)
	ON_BN_CLICKED(IDC_TEST, &CMachTesrDlg::OnBnClickedTest)
	ON_WM_TIMER()
	ON_EN_CHANGE(IDC_EDIT_NUM, &CMachTesrDlg::OnChangeEditNum)
	ON_BN_CLICKED(IDC_CHECK1, &CMachTesrDlg::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK2, &CMachTesrDlg::OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_CHECK3, &CMachTesrDlg::OnBnClickedCheck3)
	ON_BN_CLICKED(IDC_CHECK4, &CMachTesrDlg::OnBnClickedCheck4)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CMachTesrDlg::OnCbnSelchangeCombo1)
END_MESSAGE_MAP()


// CMachTesrDlg 消息处理程序

BOOL CMachTesrDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	CFont EditFont;
	EditFont.CreatePointFont(135,_T("宋体"));
	m_Zynq.SetFont(&EditFont);

	GetDlgItem(IDC_TEST)->EnableWindow(false);

	m_Num.SetLimitText(15);
	CRect rect;
	m_List.GetClientRect(&rect);

	// 为列表视图控件添加全行选中和栅格风格   
	m_List.SetExtendedStyle(m_List.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	//设置行高
	CImageList   m_l;
	m_l.Create(1,25, TRUE | ILC_COLOR32, 1, 0);
	m_List.SetImageList(&m_l, LVSIL_SMALL);

	m_List.InsertColumn(0, _T("测试项"), LVCFMT_CENTER, 105, 1);
	m_List.InsertColumn(1, _T("预期结果"), LVCFMT_CENTER, 350, 2);
	m_List.InsertColumn(2, _T("测试结果"), LVCFMT_CENTER, 350, 3);

	m_List.SetFontHW(15,8);

	this->SetWindowText(_T("FV01M整机终检软件V1.2.1"));

	m_cbCom.InsertString(0, _T("通道1"));
	m_cbCom.InsertString(1, _T("通道2"));

	Init();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMachTesrDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMachTesrDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMachTesrDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMachTesrDlg::Init() {

	m_List.DeleteAllItems();
	/*读取上一次的配置*/
	GetPrivateProfileString(_T("版本号"), _T("MAIN"), _T(""), StrZunq.GetBuffer(MAX_LENGTH), MAX_LENGTH, Path);
	StrZunq.ReleaseBuffer();
	m_List.InsertItem(0, _T("主版本号"));
	m_List.SetItemText(0, 1, StrZunq);
	m_Zynq.SetWindowTextW(StrZunq);

	m_Check1.SetCheck(0);
	m_Check2.SetCheck(0);
	m_Check3.SetCheck(0);
	m_Check4.SetCheck(0);

	/*获取测试项全部信息*/
	m_AnalysisIni.GetSectionAllKey(Path,_T("测试项"));		
	for (int i = 0; i < m_AnalysisIni.gKeyName.nKeyNum; i++) {
		int line = m_List.GetItemCount();
		m_List.InsertItem(line, m_AnalysisIni.gKeyName.szKeyBuf[i]);
		m_List.SetItemText(line, 1, m_AnalysisIni.gKeyName.szValue[i]);
		CString uTmep(m_AnalysisIni.gKeyName.szKeyBuf[i]);

		if (0 == uTmep.CompareNoCase(_T("车道线信息"))) {
			m_Check1.SetCheck(1);
		}
		else if (0 == uTmep.CompareNoCase(_T("目标信息"))){
			m_Check3.SetCheck(1); 
		}
		else if (0 == uTmep.CompareNoCase(_T("目标预警"))) {
			m_Check4.SetCheck(1);
		}
		else if (0 == uTmep.CompareNoCase(_T("车道线预警"))){
			m_Check2.SetCheck(1);
		}
	}
	int line = m_List.GetItemCount();
	m_List.InsertItem(line, _T("写入SN码"));
	
}

unsigned int CMachTesrDlg::TestThread(void* param)
{
	CMachTesrDlg *dlg = (CMachTesrDlg*)AfxGetApp()->GetMainWnd();	//获取主窗口指针
	CString CanZynq;
	CString SN;
	dlg->m_Display.SetWindowTextW(_T("正在测试......"));

	if (!dlg->m_CanComm.GetZynqWorkingStatus()) {
		AfxMessageBox(_T("请给Zynq上电\n"));
		goto ERR;
	}
	dlg->m_CanComm.ClearCanBuffer();
	CanZynq = dlg->m_CanComm.GetVer(MAJOR_VER_NUM);
	if (CanZynq.IsEmpty()) {
		AfxMessageBox(_T("主版本号检测失败\n"));
		goto ERR;
	}
	if (0 == dlg->StrZunq.CompareNoCase(CanZynq)) {
		dlg->m_List.SetItemColor(2, 0, RGB(0, 255, 0));
		dlg->m_List.SetItemText(0, 2, CanZynq);
		dlg->m_List.RedrawWindow();
	}
	else
	{
		dlg->m_List.SetItemColor(2, 0, RGB(255, 0, 0));
		dlg->m_List.SetItemText(0, 2, CanZynq);
		dlg->m_List.RedrawWindow();
		dlg->PassFlag = false;
	}
	dlg->ComparedFrame();

	if (!dlg->m_CanComm.Switch2Test()) {
		goto ERR;
	}
	
	
	if(!dlg->m_CanComm.WriteSN(dlg->c_Num)) {

		int line = dlg->m_List.GetItemCount() - 1;
		dlg->m_List.SetItemColor(2, line, RGB(255, 0, 0));
		dlg->m_List.SetItemText(line, 2, _T("fail"));
		dlg->SaveData.Append(_T("fail,"));
		dlg->m_List.RedrawWindow();
		dlg->PassFlag = false;
	}
	else {
		SN = dlg->m_CanComm.GetVer(SN_NUMBER);
		if (0 == SN.CompareNoCase(dlg->c_Num)) {
			int line = dlg->m_List.GetItemCount() - 1;
			dlg->m_List.SetItemColor(2, line, RGB(0, 255, 0));
			dlg->m_List.SetItemText(line, 2, SN);
			dlg->m_List.RedrawWindow();
			dlg->SaveData.Append(_T("pass,"));
		}
		else
		{
			int line = dlg->m_List.GetItemCount() - 1;
			dlg->m_List.SetItemColor(2, line, RGB(255, 0, 0));
			dlg->m_List.SetItemText(line, 2, _T("fail"));
			dlg->m_List.RedrawWindow();
			dlg->PassFlag = false;
			dlg->SaveData.Append(_T("fail,"));
		}
	}

	if (dlg->PassFlag == false) {
		dlg->SaveData = dlg->c_Num + _T(",Fail,") + CanZynq + _T(",") + dlg->SaveData;
	}
	else
	{
		dlg->SaveData = dlg->c_Num + _T(",Pass,") + CanZynq + _T(",") + dlg->SaveData;
	}

	/*保存结果*/

	if (dlg->m_SaveTestDate.WriteToFile(dlg->SaveDataName,dlg->SaveData) < 0) {

		AfxMessageBox(_T("结果保存失败\n"));
	}

	dlg->m_Num.SetWindowTextW(_T(""));
	dlg->SetDlgItemText(IDC_CIN, _T("DUT SN码:"));
	dlg->GotoDlgCtrl(dlg->GetDlgItem(IDC_EDIT_NUM));

	dlg->m_Display.SetWindowTextW(_T("测试完成！"));
	dlg->TestRunFlag = false;
	return true;
ERR:
	dlg->m_Num.SetWindowTextW(_T(""));
	dlg->SetDlgItemText(IDC_CIN, _T("DUT SN码:"));
	dlg->GotoDlgCtrl(dlg->GetDlgItem(IDC_EDIT_NUM));
	AfxMessageBox(_T("测试失败！"));
	dlg->m_Display.SetWindowTextW(_T("测试失败！"));
	dlg->TestRunFlag = false;
	return false;
}

void CMachTesrDlg::ComparedFrame()
{

	INT Ret;
	INT uTemp = 0;
	FrameData c_FrameData;;
	vector<int> vec;
	vector<int>::iterator ret;

	SetTimer(TIME_OUT, 20000, NULL);
	while (TimeOutFlag)
	{
		if (m_Count == 0) {
			KillTimer(TIME_OUT);
			break;
		}
		memset(&c_FrameData, 0, sizeof(FrameData));
		c_FrameData = m_CanComm.RecvFrame(nCANIndex);
		if (0 == c_FrameData.ID) {
			continue;
		}
		for (int i = 0; i < m_Count; i++)
		{
			if (c_FrameData.ID == m_FrameData[i].ID &&  c_FrameData.DataLen == m_FrameData[i].DataLen) {

				switch (c_FrameData.ID)
				{
				case 0x620:
					if (c_FrameData.Data[0] >  0x00) {
						m_List.SetItemColor(2, (i + 1), RGB(0, 255, 0));
						m_List.SetItemText((i + 1), 2, _T("pass"));
						vec.push_back(i);
						if (!ObjNumbFlag) {
							uTemp++;
						}
						ObjNumbFlag = true;
					}
					else
					{
					}
					break;
				case 0x621:
					Ret = ((c_FrameData.Data[0] & 0x06) >> 1);
					if (0 == Ret) {
						m_List.SetItemColor(2, (i + 1), RGB(0, 255, 0));
						m_List.SetItemText((i + 1), 2, _T("pass"));
						vec.push_back(i);
						if (!CarWarnFlag) {
							uTemp++;
						}
						CarWarnFlag = true;
					}
					break;
				case 0x623:
					Ret = (((c_FrameData.Data[0] & 0x80) >> 7) | ((c_FrameData.Data[1] & 0x80) >> 7));

					if (!Ret) {
						m_List.SetItemColor(2, (i + 1), RGB(0, 255, 0));
						m_List.SetItemText((i + 1), 2, _T("pass"));
						vec.push_back(i);
						if (!CarLineFlag) {
							uTemp++;
						}
						CarLineFlag = true;
						
					}
					else
					{
						printf("车道线信息测试失败\n");
					}
					break;
				case 0x624:
					break;
				default:
					break;
				}
			}
		}
		if (uTemp >= m_Count) {

			
			KillTimer(TIME_OUT);
			break;

		}

	}
	if (uTemp != m_Count)
	{
		PassFlag = false;
	}
	else
	{
		PassFlag = false;
	}
	for (int i = 0; i < m_Count; i++)
	{
		ret = find(vec.begin(), vec.end(), i);
		if (ret == vec.end())
		{
			/*该值不存在*/
			m_List.SetItemColor(2, (i + 1), RGB(255, 0, 0));
			m_List.SetItemText((i + 1), 2, _T("fail"));
			SaveData.Append(_T("fail,"));
		}
		else {
			SaveData.Append(_T("pass,"));
		}
	}
	m_List.RedrawWindow();
}

void CMachTesrDlg::ResourceReset() {

	m_CanComm.ClearCanBuffer();
	/*获取测试项全部信息*/
	SaveDataHand.Empty();
	m_AnalysisIni.GetSectionAllKey(Path, _T("测试项"));
	int i = 0;
	for (i = 0; i < m_AnalysisIni.gKeyName.nKeyNum; i++) {
		SaveDataHand.Append(m_AnalysisIni.gKeyName.szKeyBuf[i]);
		SaveDataHand = SaveDataHand + _T(",");
	}

	m_SaveTestDate.ChangeHead(SaveDataName, SaveDataHand);
	INT Count = i;
	if (m_FrameData != NULL)
	{
		free(m_FrameData);
	}
	m_FrameData = (FrameData*)malloc(sizeof(FrameData)*Count);
	m_Count = Count;
	printf("count---[%d]\n", m_Count);
	memset(m_FrameData, 0, sizeof(FrameData)*Count);
	/*填充m_FrameData*/
	if (!m_Parsing.FillingFrameData1(m_FrameData)) {
		return;
	}

	TimeOutFlag = true;

	m_List.SetItemColor(2, 0, RGB(255, 255, 255));
	m_List.SetItemText(0, 2, _T(""));
	m_List.RedrawWindow();
	m_List.SetItemColor(2, 1, RGB(255, 255, 255));
	m_List.SetItemText(1, 2, _T(""));
	m_List.RedrawWindow();

	for (int i = 2; i < m_Count + 2; i++)
	{
		m_List.SetItemColor(2, i, RGB(255, 255, 255));
		m_List.SetItemText(i, 2, _T(""));
		m_List.RedrawWindow();
	}
	int line = m_List.GetItemCount() - 1;
	m_List.SetItemText(line, 1, c_Num);
	
	//AfxMessageBox(SaveDataHand);
	SaveData.Empty();

	CarLineFlag = false;
	CarWarnFlag = false;
	ObjNumbFlag = false;
	PassFlag = true;
}

void CMachTesrDlg::OnBnClickedInit()
{
	if (InitDoneFlag) {
		m_cbCom.EnableWindow(1);
		m_CanComm.UnInitCan();
		InitDoneFlag = false;
		GetDlgItem(IDC_INIT)->SetWindowText(_T("初始化设备"));
		return;
	}
	if (0 == strCBText.CompareNoCase(_T("通道1")))
	{
		nCANIndex = 0;
	}
	else if (0 == strCBText.CompareNoCase(_T("通道2")))
	{
		nCANIndex = 1;
	}
	else if (0 == strCBText.CompareNoCase(_T("")))
	{
		AfxMessageBox(_T("请选择接收通道\n"));
		return;
	}
	if (!m_CanComm.InitCan()) {
		return;
	}
	/*获取测试项全部信息*/
	m_AnalysisIni.GetSectionAllKey(Path,_T("测试项"));
	for (int i = 0; i < m_AnalysisIni.gKeyName.nKeyNum; i++) {
		SaveDataHand.Append(m_AnalysisIni.gKeyName.szKeyBuf[i]);
		SaveDataHand = SaveDataHand + _T(",");

	}
	m_SaveTestDate.m_hand = SaveDataHand;
	SaveDataName = m_SaveTestDate.CreatFile(SaveDataHand);


	m_cbCom.EnableWindow(0);

	GetDlgItem(IDC_INIT)->SetWindowText(_T("关闭设备"));
	AfxMessageBox(_T("初始化成功\n"));
	printf("Init successful!\n");
	GotoDlgCtrl(GetDlgItem(IDC_EDIT_NUM));
	InitDoneFlag = true;
}

void CMachTesrDlg::OnBnClickedChangeZynq()
{
	// TODO: 在此添加控件通知处理程序代码
	
	m_Zynq.GetWindowText(StrZunq);
	if (StrZunq == _T(""))
	{
		GetPrivateProfileString(_T("版本号"), _T("MAIN"), _T(""), StrZunq.GetBuffer(MAX_LENGTH), MAX_LENGTH, Path);
		StrZunq.ReleaseBuffer();
		m_Zynq.SetWindowTextW(StrZunq);
		AfxMessageBox(_T("请输入正确的版本号\n"));
		return;
	}
	WritePrivateProfileString(_T("版本号"), _T("MAIN"), StrZunq, Path);
	m_List.SetItemText(0, 1, StrZunq);
}

void CMachTesrDlg::OnRclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	int index = pNMItemActivate->iItem;
	if (index == -1)
		return;
	HMENU hMenu = ::CreatePopupMenu();
	AppendMenu(hMenu, MF_STRING, 10001, _T("Remove"));
	CPoint pt;
	GetCursorPos(&pt); //获得当前鼠标位置
	UINT Cmd = (UINT)::TrackPopupMenu(hMenu, TPM_LEFTALIGN | TPM_RETURNCMD, pt.x, pt.y, 0, m_hWnd, NULL);//弹出菜单

	switch (Cmd)//响应点击的菜单
	{
	case 10001:
		RemoveLineData();
		break;
	}
}

void CMachTesrDlg::RemoveLineData() {

	int nIdx = m_List.GetSelectionMark();
	if (nIdx < 0)
	{
	}
	else if (0 == nIdx)  {
		AfxMessageBox(_T("当前内容不可删除"));
	}
	else
	{
		CString s = m_List.GetItemText(nIdx, 0);
		CString s1 = m_List.GetItemText(nIdx, 1);
		int ret = WritePrivateProfileString(_T("测试项"), s, NULL, Path);
		m_List.DeleteItem(nIdx);

		if (0 == s.CompareNoCase(_T("车道线信息"))) {
			m_Check1.SetCheck(0);
		}
		else if (0 == s.CompareNoCase(_T("目标信息"))) {
			m_Check3.SetCheck(0);
		}
		else if (0 == s.CompareNoCase(_T("目标预警"))) {
			m_Check4.SetCheck(0);
		}
		else if (0 == s.CompareNoCase(_T("车道线预警"))) {
			m_Check2.SetCheck(0);
		}
		printf("%d\n",nIdx);
	}
}

void CMachTesrDlg::OnBnClickedTest()
{
	
	if (!InitDoneFlag) {
		AfxMessageBox(_T("系统未初始化\n"));
		return;
	}
	else if (TestRunFlag) {
		AfxMessageBox(_T("测试正在进行中,无法进行下一次测试\n"));
		return;
	}
	m_Num.GetWindowText(c_Num);
	if (c_Num == "") {
		AfxMessageBox(_T("未录入编码！"));
		GotoDlgCtrl(GetDlgItem(IDC_EDIT_NUM));
		return;
	}

	
	GetDlgItem(IDC_TEST)->EnableWindow(false);
	TestRunFlag = true;
	ResourceReset();
	AfxBeginThread(TestThread, 0);

}

void CMachTesrDlg::OnTimer(UINT_PTR nIDEvent)
{
	switch(nIDEvent)
	{
	case TIME_OUT:
		atrace("TimeOutFlag.");
		KillTimer(TIME_OUT);
		TimeOutFlag = false;
		break;
	default:
		break;
	}
	
	CDialogEx::OnTimer(nIDEvent);
}

void CMachTesrDlg::OnChangeEditNum()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码.
	static int count = 0;
	m_Num.GetWindowTextW(StrNum1);
	if (15 == StrNum1.GetLength()) {
		m_Num.GetWindowTextW(StrNum[count++]);
		printf("%d\n", count);
		if (count == 2) {
			if (StrNum[0] == StrNum[1]) {
				GetDlgItem(IDC_TEST)->EnableWindow(true);
				count = 0;
				return;
			}
			else
			{
				GotoDlgCtrl(GetDlgItem(IDC_EDIT_NUM));
				GetDlgItem(IDC_TEST)->EnableWindow(false);
				count = 0;
				AfxMessageBox(_T("输入编码不一致,请重新输入!\n"));
				return;
			}
		}
		GotoDlgCtrl(GetDlgItem(IDC_EDIT_NUM));
	}
}

void CMachTesrDlg::OnBnClickedCheck1()
{
	// TODO: 在此添加控件通知处理程序代码
	int line = m_List.GetItemCount();
	if (1 == m_Check1.GetCheck()) {
		WritePrivateProfileString(_T("测试项"), _T("车道线信息"), _T("ID:623"), Path);
		m_List.InsertItem(line-1, _T("车道线信息"));
		m_List.SetItemText(line-1, 1, _T("ID:623"));
	}
	else
	{
		WritePrivateProfileString(_T("测试项"), _T("车道线信息"), NULL, Path);
		for (size_t i = 1; i < line; i++)
		{
			CString s = m_List.GetItemText(i, 0);
			if (0 == s.CompareNoCase(_T("车道线信息")))
			{
				m_List.DeleteItem(i);
				break;
			}
		}
	}
}

void CMachTesrDlg::OnBnClickedCheck2()
{
	// TODO: 在此添加控件通知处理程序代码
	int line = m_List.GetItemCount();
	if (1 == m_Check2.GetCheck()) {
		WritePrivateProfileString(_T("测试项"), _T("车道线预警"), _T("ID:621"), Path);
		m_List.InsertItem(line-1, _T("车道线预警"));
		m_List.SetItemText(line-1, 1, _T("ID:621"));
	}
	else {
		WritePrivateProfileString(_T("测试项"), _T("车道线预警"), NULL, Path);
		for (size_t i = 1; i < line; i++)
		{
			CString s = m_List.GetItemText(i, 0);
			if (0 == s.CompareNoCase(_T("车道线预警")))
			{
				m_List.DeleteItem(i);
				break;
			}
		}
	}
}

void CMachTesrDlg::OnBnClickedCheck3()
{
	// TODO: 在此添加控件通知处理程序代码
	int line = m_List.GetItemCount();
	if (1 == m_Check3.GetCheck()) {
		WritePrivateProfileString(_T("测试项"), _T("目标信息"), _T("ID:620"), Path);
		m_List.InsertItem(line-1, _T("目标信息"));
		m_List.SetItemText(line-1, 1, _T("ID:620"));
	}
	else {
		WritePrivateProfileString(_T("测试项"), _T("目标信息"), NULL, Path);
		for (size_t i = 1; i < line; i++)
		{
			CString s = m_List.GetItemText(i, 0);
			if (0 == s.CompareNoCase(_T("目标信息")))
			{
				m_List.DeleteItem(i);
				break;
			}
		}
	}
}

void CMachTesrDlg::OnBnClickedCheck4()
{
	// TODO: 在此添加控件通知处理程序代码
	int line = m_List.GetItemCount();
	if (1 == m_Check4.GetCheck()) {
		WritePrivateProfileString(_T("测试项"), _T("目标预警"), _T("ID:624"), Path);
		m_List.InsertItem(line-1, _T("目标预警"));
		m_List.SetItemText(line-1, 1, _T("ID:624"));
	}
	else {
		WritePrivateProfileString(_T("测试项"), _T("目标预警"), NULL, Path);
		for (size_t i = 1; i < line; i++)
		{
			CString s = m_List.GetItemText(i, 0);
			if (0 == s.CompareNoCase(_T("目标预警")))
			{
				m_List.DeleteItem(i);
				break;
			}
		}
	}
}

CString CMachTesrDlg::GetWorkDir()
{
	wchar_t  pFileName[MAX_PATH];
	int nPos = GetCurrentDirectory(MAX_PATH, pFileName);

	CString csFullPath(pFileName);
	if (nPos < 0)
		return CString("");
	else
		return csFullPath;
}

//防止按回车键退出程序
void CMachTesrDlg::OnOK()	//CexeDemoDlg为防止退出的对话框
{

}


void CMachTesrDlg::OnCbnSelchangeCombo1()
{
	// TODO: 在此添加控件通知处理程序代码

	int nIndex = m_cbCom.GetCurSel();
	m_cbCom.GetLBText(nIndex, strCBText);

}
