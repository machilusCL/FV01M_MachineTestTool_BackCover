#pragma once
#include "ControlCAN.h"
#include "trace.h"

#define CAN_PRODUCT 0
#define CAN_NAVIGATE 1

#define CAN_ZYNQ	0x2
#define CAN_MCU		0x3
#define MAJOR_VER_NUM 0x21
#define SN_NUMBER 0x20


enum eTIMEOUT{
	SWITCH_MODE = 0,
};
typedef struct _tResult {
	double Par_Result[3][3];
	double Cal_Result[5];
}tResult;

typedef struct _FrameData {
	UINT ID;
	BYTE DataLen;
	BYTE Data[8];
}FrameData;


class CCanComm
{
private:
	int SetBaudRate(int baudrate, VCI_INIT_CONFIG_EX *config_ex);
	tResult *date;

public:
	CCanComm(void);
	~CCanComm(void);

	bool InitCan();
	bool SendGetFrameCmd();
	bool Switch2Test();
	bool UnInitCan();
	void ClearCanBuffer();
	bool GetZynqWorkingStatus();
	bool WriteSN(CString Number);

	CString  GetVer(BYTE Verfalg);
	FrameData RecvFrame(UINT nCANIndex);

	UINT timecount = 0;
};

