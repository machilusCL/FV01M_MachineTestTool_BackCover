#include "stdafx.h"
#include "CanComm.h"


#define ID_TIMER 1

typedef  const struct {
  int BAUD_RATE; 
  unsigned char   SJW;
  unsigned char   BS1;
  unsigned char   BS2;
  unsigned short  PreScale;
} s_CAN_BaudRate;

/*
Used for Hardware (CLK=36MHz):
1. ViewTool Ginkgo USB-CAN Bus Adapter: VTG202A
2. ViewTool Ginkgo USB-CAN Interface: VTG203B

*/
s_CAN_BaudRate  CAN_BaudRateInitTab[]= {     
{1000,1,2,1,9},       // 1M (1000K)
{900,1,5,2,5},       // 900K
{800,1,10,4,3},       // 800K
{666,1,7,1,6},       // 666K
{600,1,3,1,12},       // 600K
{500,1,4,1,12},       // 500K
{400,1,7,1,10},       // 400K
{300,1,6,1,15},      // 300K
{250,1,6,1,18},      // 250K
{225,1,6,1,20},      // 225K
{200,1,15,2,10},      // 200K
{160,1,12,2,15},      // 160K
{150,1,6,1,30},      // 150K
{144,1,3,1,50},      // 144K
{125,1,6,1,36},     // 125K
{120,1,12,2,20},     // 120K
{100,1,6,1,45},      // 100K
{90,1,6,1,50},     // 90K
{80,1,4,1,75},     // 80K
{75,1,6,1,60},      // 75K
{60,1,6,1,75},      // 60K
{50,1,6,1,90},      // 50K
{40,1,7,1,100},      // 40K
{30,1,6,1,150},     // 30K
{20,1,6,1,225},     // 20K
};


CCanComm::CCanComm(void)
{
}


CCanComm::~CCanComm(void)
{
}

bool CCanComm::InitCan(){
	int DevNum,Status;

    //Scan device
    DevNum = VCI_ScanDevice(1);
	if(DevNum > 0){
		atrace("VCI_ScanDevice OK");
    }else{
        AfxMessageBox(_T("Ginkgo����δ����\n"));
        return false;
    }

	Status = VCI_OpenDevice(VCI_USBCAN2,0,0);
	if(Status==STATUS_ERR){
		AfxMessageBox(_T("��can�豸ʧ��\n"));
		return false;
	}else{
		atrace(("Open device success!"));
	}

	for(int i=0;i<2;i++){
		VCI_INIT_CONFIG_EX	CAN_InitEx;
		CAN_InitEx.CAN_Mode = 0;
		SetBaudRate(500, &CAN_InitEx);
		CAN_InitEx.CAN_NART = 1;
		CAN_InitEx.CAN_RFLM = 0;
		CAN_InitEx.CAN_TXFP = 1;
		CAN_InitEx.CAN_RELAY = 0;

		Status = VCI_InitCANEx(VCI_USBCAN2,0,i,&CAN_InitEx);
		if(Status==STATUS_ERR){
			 AfxMessageBox(_T("��ʼ��can�豸ʧ��\n"));
			 return false;
		}else{
			atrace("Init device success!");
		}
		  //Set filter
		VCI_FILTER_CONFIG CAN_FilterConfig;
		CAN_FilterConfig.FilterIndex = 0;
		CAN_FilterConfig.Enable = 1;		//Enable
		CAN_FilterConfig.ExtFrame = 0;
		CAN_FilterConfig.FilterMode = 0;
		CAN_FilterConfig.ID_IDE = 0;
		CAN_FilterConfig.ID_RTR = 0;
		CAN_FilterConfig.ID_Std_Ext = 0;
		CAN_FilterConfig.MASK_IDE = 0;
		CAN_FilterConfig.MASK_RTR = 0;
		CAN_FilterConfig.MASK_Std_Ext = 0;
		Status = VCI_SetFilter(VCI_USBCAN2,0,i,&CAN_FilterConfig);
		for(int j=1; j<=13; j++)
		{
			CAN_FilterConfig.FilterIndex = j;
			Status = VCI_SetFilter(VCI_USBCAN2,0,i,&CAN_FilterConfig);
		}
		if(Status==STATUS_ERR){
			AfxMessageBox(_T("���ù�����ʧ��\n"));
			return false;
		}else{
			atrace("Set filter success!");
		}

		Status = VCI_StartCAN(VCI_USBCAN2,0,i);
		if(Status==STATUS_ERR){
			AfxMessageBox(_T("Start CAN failed!\n"));
			return false;
		}else{
			atrace("Start CAN success!");
		}
	}
	return true;
}

bool CCanComm::UnInitCan(){
	return VCI_CloseDevice(VCI_USBCAN2,0)?true:false;
}
 
/*
SetBaudRate:  Set CAN protocol communication baudrate
Parameter:  
  baudrate: unit: Khz, range: 20-1000 -> 20Khz-1000Khz
Return: 
  1: success
  0: failed (no matched)

*/
int CCanComm::SetBaudRate(int baudrate, VCI_INIT_CONFIG_EX *config_ex)
{
	int i, ret=0; 
	for(i=0; i<sizeof(CAN_BaudRateInitTab); i++)
	{
		if(CAN_BaudRateInitTab[i].BAUD_RATE == baudrate)
		{
			config_ex->CAN_SJW = CAN_BaudRateInitTab[i].SJW;
			config_ex->CAN_BS1 = CAN_BaudRateInitTab[i].BS1;
			config_ex->CAN_BS2 = CAN_BaudRateInitTab[i].BS2;
			config_ex->CAN_BRP = CAN_BaudRateInitTab[i].PreScale;
		    atrace("Set Baud Rate = %d Khz", baudrate);
			ret = 1; 
			break;
		}
	}
	return ret; 
}

bool CCanComm::SendGetFrameCmd(){
	uint32_t uRet;

	VCI_CAN_OBJ Frame_Pic;
	Frame_Pic.RemoteFlag = 0;
	Frame_Pic.ExternFlag = 0;
	Frame_Pic.SendType = 0;
	Frame_Pic.DataLen = 4;
	Frame_Pic.ID = 0x7DF;
	Frame_Pic.Data[0] = 0x3;
	Frame_Pic.Data[1] = 0x13;
	Frame_Pic.Data[2] = 0x9;
	Frame_Pic.Data[3] = 0xe;
	uRet = VCI_Transmit(VCI_USBCAN2, 0, CAN_PRODUCT, &Frame_Pic, 1);
	if (0 == uRet)
	{
		AfxMessageBox(_T("Can����ʧ��\n"));
		return false;
	}
	return true;
}

bool CCanComm::Switch2Test(int timeout1, int timeout2){

	uint32_t uRet;
	uint32_t uLen;
	VCI_CAN_OBJ RcvFrame;

	VCI_CAN_OBJ Frame_Pic;
	Frame_Pic.RemoteFlag = 0;
	Frame_Pic.ExternFlag = 0;
	Frame_Pic.SendType = 0;
	Frame_Pic.DataLen = 4;
	Frame_Pic.ID = 0x7DF;
	Frame_Pic.Data[0] = 0x3;
	Frame_Pic.Data[1] = 0x10;
	Frame_Pic.Data[2] = 0x9;
	Frame_Pic.Data[3] = 0x01;
	CTime t3 = CTime::GetCurrentTime();
	while (true)
	{
		uRet = VCI_Transmit(VCI_USBCAN2, 0, CAN_PRODUCT, &Frame_Pic, 1);
		if (0 < uRet)
		{
			//AfxMessageBox(_T("Can����ʧ��\n"));
			//return false;
			break;
		}
		CTime t4 = CTime::GetCurrentTime();
		CTimeSpan span1 = t4 - t3;
		LONGLONG iSec1 = span1.GetTotalSeconds();
		if (iSec1 >= timeout1) {
			atrace("can rsp timeout.");
			break;
			//return false;
		}
		
		Sleep(2000);
	}

	CTime t1 = CTime::GetCurrentTime();
	while (true)
	{
		uLen = VCI_Receive(VCI_USBCAN2, 0, CAN_PRODUCT, &RcvFrame, 1);
		if (1 == uLen ) {
			if (RcvFrame.ID == 0x759 && RcvFrame.Data[0] == 0x03) {
				int s = RcvFrame.Data[1] == 0x50 ? 1 : 0;
				int a = RcvFrame.Data[2] == 0x09 ? 1 : 0;
				int w = RcvFrame.Data[3] == 0x01 ? 1 : 0;
				if ( s && a && w) {
					atrace("switch to test mode success.");
					return true;
				}
			}
			else if (RcvFrame.ID == 0x759 && RcvFrame.Data[0] == 0x7F) {
					atrace("switch to test mode failed.");
					return false;
			}
		}

		CTime t2 = CTime::GetCurrentTime();
		CTimeSpan span = t2 - t1;
		LONGLONG iSec=span.GetTotalSeconds();
		if(iSec >= 5){
			atrace("can rsp timeout.");
			return false;
		}
	}
	return true;
}

CString  CCanComm::GetVer(BYTE Verfalg)
{
	CString McuVer;
	CString Ver;
	uint32_t uRet;
	uint32_t uLen;
	uint32_t len;

	VCI_CAN_OBJ RcvFrame;
	VCI_CAN_OBJ Frame_Ver;
	Frame_Ver.RemoteFlag = 0;
	Frame_Ver.ExternFlag = 0;
	Frame_Ver.SendType = 0;
	Frame_Ver.DataLen = 4;
	Frame_Ver.ID = 0x7DF;
	Frame_Ver.Data[0] = 0x3;
	Frame_Ver.Data[1] = 0x11;
	Frame_Ver.Data[2] = 0x9;
	Frame_Ver.Data[3] = Verfalg;
	
	CTime t9 = CTime::GetCurrentTime();
	while (true)
	{
		uRet = VCI_Transmit(VCI_USBCAN2, 0, CAN_PRODUCT, &Frame_Ver, 1);
		if (0 == uRet)
		{
			/*AfxMessageBox(_T("Can����ʧ��\n"));
			return NULL;*/
		}
		if (1 == uRet) 
		{
			break;
		}
		CTime t8 = CTime::GetCurrentTime();
		CTimeSpan span = t8 - t9;
		LONGLONG iSec = span.GetTotalSeconds();
		if (iSec >= 20) {
			Ver.Append(_T("can rsp timeout."));
			atrace("can rsp timeout.");
			return NULL;
		}
		Sleep(5000);
	}
	
	CTime t1 = CTime::GetCurrentTime();
	while (true)
	{
		uLen = VCI_Receive(VCI_USBCAN2, 0, CAN_PRODUCT, &RcvFrame, 1);
		if (uLen == 1) {
			if (RcvFrame.ID == 0x759 && RcvFrame.Data[4] == Verfalg) {
				len = RcvFrame.Data[1] & 0xFF;
				for (size_t i = 0; i < 3; i++)
				{
					McuVer.Format(_T("%c"), RcvFrame.Data[i + 5]);
					Ver.Append(McuVer);
				}
				int nCntFrame = (len - 6) / 7;
				int nTemp = (len - 6) % 7;
				while (nCntFrame) {					
					memset(RcvFrame.Data, 0, 8);
					uLen = VCI_Receive(VCI_USBCAN2, 0, CAN_PRODUCT, &RcvFrame, 1);
					if (uLen == 1) {
						for (size_t i = 0; i < 7; i++)
						{
							McuVer.Format(_T("%c"), RcvFrame.Data[i + 1]);
							Ver.Append(McuVer);
						}

					}
					nCntFrame--;
				}
				while(nTemp) {
					memset(RcvFrame.Data, 0, 8);
					uLen = VCI_Receive(VCI_USBCAN2, 0, CAN_PRODUCT, &RcvFrame, 1);
					if (uLen == 1) {
						for (size_t i = 0; i < nTemp; i++)
						{
							McuVer.Format(_T("%c"), RcvFrame.Data[i + 1]);
							Ver.Append(McuVer);
						}
						break;
					}
				}
				break;
			}
			else if (RcvFrame.ID == 0x759 && RcvFrame.Data[1] == 0x7F) {
				timecount = 0;
				Ver.Append(_T("Service does not support.")); 
				return Ver;
			}
				
		}
		CTime t2 = CTime::GetCurrentTime();
		CTimeSpan span = t2 - t1;
		LONGLONG iSec = span.GetTotalSeconds();
		if (iSec >= 5) {
			if (timecount < 3) {
				timecount++;
				atrace("timecount:%d\n", timecount);
				return GetVer(Verfalg);
			}else
			{
				timecount = 0;
				Ver.Append(_T("can rsp timeout."));
				atrace("can rsp timeout.");
				return Ver;
			}
		}
	}
	
	return Ver;
}

FrameData CCanComm::RecvFrame()
{
	uint32_t uRet;
	FrameData RecvData;
	VCI_CAN_OBJ Frame_Recv;
	
	memset(&Frame_Recv.Data[0], 0, sizeof(BYTE)*8);
	memset(&RecvData, 0, sizeof(FrameData));
	uRet = VCI_Receive(VCI_USBCAN2, 0, CAN_NAVIGATE, &Frame_Recv, 1);
	if (uRet == 1 ) {
		RecvData.ID = Frame_Recv.ID;
		RecvData.DataLen = Frame_Recv.DataLen;
		memcpy(RecvData.Data, Frame_Recv.Data, Frame_Recv.DataLen);
	}
	return RecvData;
}

void  CCanComm::ClearCanBuffer() {

	int ret = VCI_ClearBuffer(VCI_USBCAN2, 0, CAN_NAVIGATE);
	if (ret == 1) {
		printf("Clear buffer success!\n");
	}
	else {
		printf("Clear buffer failed!\n");
	}
	ret = VCI_ClearBuffer(VCI_USBCAN2, 0, CAN_PRODUCT);
	if (ret == 1) {
		printf("Clear buffer success!\n");
	}
	else {
		printf("Clear buffer failed!\n");
	}

}